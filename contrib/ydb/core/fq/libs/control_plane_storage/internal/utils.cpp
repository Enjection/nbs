#include "utils.h"

#include <library/cpp/json/yson/json2yson.h>

#include <contrib/ydb/core/metering/bill_record.h>
#include <contrib/ydb/core/metering/metering.h>
#include <contrib/ydb/public/lib/fq/scope.h>

namespace NFq {

using NYdb::NFq::TScope;

NYql::TIssues ValidateWriteResultData(const TString& resultId, const Ydb::ResultSet& resultSet, const TInstant& deadline, const TDuration& ttl)
{
    NYql::TIssues issues;
    if (!resultId) {
        issues.AddIssue(MakeErrorIssue(TIssuesIds::BAD_REQUEST, "result id is not specified"));
    }

    if (resultSet.rows().size() == 0) {
        issues.AddIssue(MakeErrorIssue(TIssuesIds::BAD_REQUEST, "result set rows count is empty"));
    }

    const auto hardLimit = TInstant::Now() + ttl;
    if (deadline > hardLimit) {
        issues.AddIssue(MakeErrorIssue(TIssuesIds::BAD_REQUEST, "deadline " + deadline.ToString() + " must be less than " + hardLimit.ToString()));
    }

    return issues;
}

NYql::TIssues ValidateGetTask(const TString& owner, const TString& hostName)
{
    NYql::TIssues issues;
    if (!owner) {
        issues.AddIssue(MakeErrorIssue(TIssuesIds::BAD_REQUEST, "owner is not specified"));
    }

    if (!hostName) {
        issues.AddIssue(MakeErrorIssue(TIssuesIds::BAD_REQUEST, "host name is not specified"));
    }

    return issues;
}

NYql::TIssues ValidatePingTask(const TString& scope, const TString& queryId, const TString& owner, const TInstant& deadline, const TDuration& ttl)
{
    NYql::TIssues issues;
    if (!scope) {
        issues.AddIssue(MakeErrorIssue(TIssuesIds::BAD_REQUEST, "scope is not specified"));
    }

    if (!queryId) {
        issues.AddIssue(MakeErrorIssue(TIssuesIds::BAD_REQUEST, "query id is not specified"));
    }

    if (!owner) {
        issues.AddIssue(MakeErrorIssue(TIssuesIds::BAD_REQUEST, "owner is not specified"));
    }

    const auto hardLimit = TInstant::Now() + ttl;
    if (deadline > hardLimit) {
        issues.AddIssue(MakeErrorIssue(TIssuesIds::BAD_REQUEST, "deadline " + deadline.ToString() + " must be less than " + hardLimit.ToString()));
    }

    return issues;
}

NYql::TIssues ValidateNodesHealthCheck(
    const TString& tenant,
    const TString& instanceId,
    const TString& hostName
    )
{
    NYql::TIssues issues;
    if (!tenant) {
        issues.AddIssue(MakeErrorIssue(TIssuesIds::BAD_REQUEST, "tenant is not specified"));
    }

    if (!instanceId) {
        issues.AddIssue(MakeErrorIssue(TIssuesIds::BAD_REQUEST, "instance id is not specified"));
    }

    if (!hostName) {
        issues.AddIssue(MakeErrorIssue(TIssuesIds::BAD_REQUEST, "hostName is not specified"));
    }

    return issues;
}

NYql::TIssues ValidateCreateOrDeleteRateLimiterResource(const TString& queryId, const TString& scope, const TString& tenant, const TString& owner)
{
    NYql::TIssues issues;
    if (!queryId) {
        issues.AddIssue(MakeErrorIssue(TIssuesIds::BAD_REQUEST, "query id is not specified"));
    }

    if (!scope) {
        issues.AddIssue(MakeErrorIssue(TIssuesIds::BAD_REQUEST, "scope is not specified"));
    }

    if (!tenant) {
        issues.AddIssue(MakeErrorIssue(TIssuesIds::BAD_REQUEST, "tenant is not specified"));
    }

    if (!owner) {
        issues.AddIssue(MakeErrorIssue(TIssuesIds::BAD_REQUEST, "owner is not specified"));
    }

    return issues;
}

std::vector<TString> GetMeteringRecords(const TString& statistics, bool billable, const TString& jobId, const TString& scope, const TString& sourceId) {

    std::vector<TString> result;
    NJson::TJsonReaderConfig jsonConfig;
    NJson::TJsonValue stat;

    if (!billable) {
        return result;
    }

    ui64 ingress = 0;
    if (NJson::ReadJsonTree(statistics, &jsonConfig, &stat)) {
        for (const auto& graph : stat.GetMapSafe()) {
            // all stats should expose IngressBytes now
            if (auto* ingressNode = graph.second.GetValueByPath("IngressBytes.sum")) {
                ingress += ingressNode->GetIntegerSafe();
            }
        }
    }

    auto ingressMBytes = (ingress + 1_MB - 1) >> 20; // round up to 1 MB boundary
    if (ingressMBytes < 10) {
        ingressMBytes = 10;
    }

    auto now = Now();
    result.emplace_back(TBillRecord()
        .Id(jobId + "_i")
        .Schema("yq.ingress.v1")
        .FolderId(TScope(scope).ParseFolder())
        .SourceWt(now)
        .SourceId(sourceId)
        .Usage(TBillRecord::TUsage()
            .Type(TBillRecord::TUsage::EType::Delta)
            .Unit(TBillRecord::TUsage::EUnit::MByte)
            .Quantity(ingressMBytes)
            .Start(now)
            .Finish(now)
        )
        .ToString()
    );

    return result;
}

void RemapValue(NYson::TYsonWriter& writer, const NJson::TJsonValue& node, const TString& key) {
    writer.OnKeyedItem(key);
    if (auto* keyNode = node.GetValueByPath(key)) {
        switch (keyNode->GetType()) {
        case NJson::JSON_BOOLEAN:
            writer.OnBooleanScalar(keyNode->GetBoolean());
            break;
        case NJson::JSON_INTEGER:
            writer.OnInt64Scalar(keyNode->GetInteger());
            break;
        case NJson::JSON_DOUBLE:
            writer.OnDoubleScalar(keyNode->GetDouble());
            break;
        case NJson::JSON_STRING:
        default:
            writer.OnStringScalar(keyNode->GetStringSafe());
            break;
        }
    } else {
        writer.OnStringScalar("-");
    }
}

void RemapNode(NYson::TYsonWriter& writer, const NJson::TJsonValue& node, const TString& path, const TString& key) {
    if (auto* subNode = node.GetValueByPath(path)) {
        writer.OnKeyedItem(key);
        writer.OnBeginMap();
            RemapValue(writer, *subNode, "sum");
            RemapValue(writer, *subNode, "count");
            RemapValue(writer, *subNode, "avg");
            RemapValue(writer, *subNode, "max");
            RemapValue(writer, *subNode, "min");
        writer.OnEndMap();
    }
}

void AggregateNode(const NJson::TJsonValue& node, const TString& path, ui64& min, ui64& max, ui64& sum, ui64& count) {
    if (node.GetType() == NJson::JSON_MAP) {
        if (auto* subNode = node.GetValueByPath(path)) {
            if (auto* keyNode = subNode->GetValueByPath("count")) {
                auto nodeCount = keyNode->GetInteger();
                if (nodeCount) {
                    if (auto* keyNode = subNode->GetValueByPath("min")) {
                        auto nodeMin = keyNode->GetInteger();
                        min = count ? std::min<ui64>(min, nodeMin) : nodeMin;
                    }
                    if (auto* keyNode = subNode->GetValueByPath("max")) {
                        auto nodeMax = keyNode->GetInteger();
                        max = count ? std::max<ui64>(max, nodeMax) : nodeMax;
                    }
                    if (auto* keyNode = subNode->GetValueByPath("sum")) {
                        sum += keyNode->GetInteger();
                    }
                    // ignore "avg"
                    count += nodeCount;
                }
            }
        }
    }
}

void AggregateNode(NYson::TYsonWriter& writer, const NJson::TJsonValue& node, const TString& path, const TString& key) {
    ui64 min = 0;
    ui64 max = 0;
    ui64 sum = 0;
    ui64 count = 0;

    if (node.GetType() == NJson::JSON_MAP) {
        for (const auto& p : node.GetMap()) {
            AggregateNode(p.second, path, min, max, sum, count);
        }
    }

    if (count) {
        writer.OnKeyedItem(key);
        writer.OnBeginMap();
            writer.OnKeyedItem("sum");
            writer.OnInt64Scalar(sum);
            writer.OnKeyedItem("count");
            writer.OnInt64Scalar(count);
            writer.OnKeyedItem("avg");
            writer.OnInt64Scalar(sum / count);
            writer.OnKeyedItem("min");
            writer.OnInt64Scalar(min);
            writer.OnKeyedItem("max");
            writer.OnInt64Scalar(max);
        writer.OnEndMap();
    }
}

TString GetPrettyStatistics(const TString& statistics) {
    TStringStream out;
    NYson::TYsonWriter writer(&out);
    writer.OnBeginMap();
    NJson::TJsonReaderConfig jsonConfig;
    NJson::TJsonValue stat;
    if (NJson::ReadJsonTree(statistics, &jsonConfig, &stat)) {

        //  EXP 
        if (stat.GetValueByPath("Columns")) {
            return statistics;
        }

        for (const auto& p : stat.GetMap()) {
            // YQv1
            if (p.first.StartsWith("Graph=") || p.first.StartsWith("Precompute=")) {
                writer.OnKeyedItem(p.first);
                writer.OnBeginMap();
                    RemapNode(writer, p.second, "StagesCount", "StagesCount");
                    RemapNode(writer, p.second, "TaskRunner.Stage=Total.TasksCount", "TasksCount");
                    RemapNode(writer, p.second, "TaskRunner.Stage=Total.BuildCpuTimeUs", "BuildCpuTimeUs");
                    RemapNode(writer, p.second, "TaskRunner.Stage=Total.ComputeCpuTimeUs", "ComputeCpuTimeUs");
                    RemapNode(writer, p.second, "TaskRunner.Stage=Total.CpuTimeUs", "CpuTimeUs");
                    RemapNode(writer, p.second, "TaskRunner.Stage=Total.SourceCpuTimeUs", "SourceCpuTimeUs");
                    RemapNode(writer, p.second, "TaskRunner.Stage=Total.IngressS3SourceBytes", "IngressObjectStorageBytes");
                    RemapNode(writer, p.second, "TaskRunner.Stage=Total.EgressS3SinkBytes", "EgressObjectStorageBytes");
                    RemapNode(writer, p.second, "TaskRunner.Stage=Total.IngressPqSourceBytes", "IngressStreamBytes");
                    RemapNode(writer, p.second, "TaskRunner.Stage=Total.EgressPqSinkBytes", "EgressStreamBytes");
                    RemapNode(writer, p.second, "IngressBytes", "IngressBytes");
                    RemapNode(writer, p.second, "EgressBytes", "EgressBytes");
                writer.OnEndMap();
            }
            // YQv2
            // if (p.first.StartsWith("Query")) 
            else {
                writer.OnKeyedItem(p.first);
                writer.OnBeginMap();
                    AggregateNode(writer, p.second, "Tasks", "Tasks");
                    AggregateNode(writer, p.second, "CpuTimeUs", "CpuTimeUs");
                    RemapNode(writer, p.second, "IngressBytes", "IngressBytes");
                    RemapNode(writer, p.second, "IngressRows", "IngressRows");
                    RemapNode(writer, p.second, "InputBytes", "InputBytes");
                    RemapNode(writer, p.second, "InputRows", "InputRows");
                    RemapNode(writer, p.second, "OutputBytes", "OutputBytes");
                    RemapNode(writer, p.second, "OutputRows", "OutputRows");
                    RemapNode(writer, p.second, "ResultBytes", "ResultBytes");
                    RemapNode(writer, p.second, "ResultRows", "ResultRows");
                    RemapNode(writer, p.second, "EgressBytes", "EgressBytes");
                    RemapNode(writer, p.second, "EgressRows", "EgressRows");
                writer.OnEndMap();
            }
        }
    }
    writer.OnEndMap();
    return NJson2Yson::ConvertYson2Json(out.Str());
}

};
