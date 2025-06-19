#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <vector>
#include <unordered_map>

class OkGeoError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class OsError : public OkGeoError {
public:
    using OkGeoError::OkGeoError;
};

class ProbeFileParseError : public OkGeoError {
public:
    using OkGeoError::OkGeoError;
};

class AnnoFileParseError : public OkGeoError {
public:
    using OkGeoError::OkGeoError;
};

class SeriesMatrixFileParseError : public OkGeoError {
public:
    using OkGeoError::OkGeoError;
};

typedef std::vector<std::string> Ids;
typedef std::unordered_map<std::string, std::string> Anno;
typedef std::vector<double> Expr;
typedef std::vector<Expr> Exprs;
typedef std::vector<Expr*> ExprPtrs;
typedef std::vector<std::vector<std::string>> Phenotype;

class ProbeExprHelper {
public:
    static void Parse(const std::string& path, Ids& probes, Ids& samples, Exprs& probe_exprs);
};

class AnnoHelper {
public:
    static void Parse(const std::string& path, int column, Anno& anno);
};

class GeneExprHelper {
public:
    static void Map(Anno& anno, Ids& probes, Exprs& probe_exprs, std::unordered_map<std::string, ExprPtrs>& gene_probe_expr_ptrs);
    static void Aggregate(std::unordered_map<std::string, ExprPtrs>& gene_probe_expr_ptrs, const std::string& method, Ids& genes, Exprs& gene_exprs);
    static void Write(Ids& probes, Ids& samples, Exprs& gene_exprs, const std::string& outfile);

protected:
    static void Mean(ExprPtrs& probe_expr_ptrs, Expr& gene_expr);
    static void Median(ExprPtrs& probe_expr_ptrs, Expr& gene_expr);
    static void Max(ExprPtrs& probe_expr_ptrs, Expr& gene_expr);
    static void Min(ExprPtrs& probe_expr_ptrs, Expr& gene_expr);
};

class PhenotypeHelper {
public:
    static void Parse(const std::string& file, Phenotype& phenotype);
    static void Write(const Phenotype& phenotype, const std::string& file);
};

#endif // HELPERS_H
