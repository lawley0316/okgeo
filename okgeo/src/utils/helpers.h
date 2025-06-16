#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <vector>
#include <unordered_map>


namespace stats {
    template<typename T>
    double median(std::vector<T> v);
}

template<typename T>
double stats::median(std::vector<T> v)
{
    if (v.empty()) throw std::invalid_argument("no median for empty data");
    std::size_t n = v.size(), m = n / 2;
    std::nth_element(v.begin(), v.begin()+m, v.end());
    if (n % 2 == 0) {
        return (*std::max_element(v.begin(), v.begin()+m) + v[m]) / 2.0;
    } else {
        return static_cast<double>(v[m]);
    }
}

class OkGeoError : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

class OsError : public OkGeoError
{
public:
    using OkGeoError::OkGeoError;
};

class ProbeFileParseError : public OkGeoError
{
public:
    using OkGeoError::OkGeoError;
};

class AnnoFileParseError : public OkGeoError
{
public:
    using OkGeoError::OkGeoError;
};

class SeriesMatrixFileParseError : public OkGeoError
{
public:
    using OkGeoError::OkGeoError;
};

typedef std::vector<std::string> Ids;
typedef std::unordered_map<std::string, std::string> Anno;
typedef std::vector<double> Expr;
typedef std::vector<Expr> Exprs;
typedef std::vector<Expr*> ExprPtrs;
typedef std::vector<std::vector<std::string>> Phenotype;

class ProbeExprHelper
{
public:
    static void parse(const std::string& path, Ids& probes, Ids& samples, Exprs& probeExprs);
};

class AnnoHelper
{
public:
    static void parse(const std::string& path, int column, Anno& anno);
};

class GeneExprHelper
{
public:
    static void map(Anno& anno, Ids& probes, Exprs& probeExprs, std::unordered_map<std::string, ExprPtrs>& geneProbeExprPtrs);
    static void merge(std::unordered_map<std::string, ExprPtrs>& geneProbeExprPtrs, const std::string& method, Ids& genes, Exprs& geneExprs);
    static void write(Ids& probes, Ids& samples, Exprs& geneExprs, const std::string& outfile);

protected:
    static void mean(ExprPtrs& probeExprPtrs, Expr& geneExpr);
    static void median(ExprPtrs& probeExprPtrs, Expr& geneExpr);
    static void max(ExprPtrs& probeExprPtrs, Expr& geneExpr);
    static void min(ExprPtrs& probeExprPtrs, Expr& geneExpr);
};

class PhenotypeHelper {
public:
    static void Parse(const std::string& file, Phenotype& phenotype);
    static void Write(const Phenotype& phenotype, const std::string& file);
};

#endif // HELPERS_H
