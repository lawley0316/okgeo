#include "tst_testgeneexprhelper.h"

#include "src/utils/helpers.h"

TestGeneExprHelper::TestGeneExprHelper(QObject *parent)
    : QObject{parent} {}

void TestGeneExprHelper::test_Aggregate_Mean() {
    Ids probes = {"probe0", "probe1", "probe2", "probe3", "probe4", "probe5"};
    Exprs probe_exprs = {
        {1.0, 2.0, 3.0},    // probe0
        {4.0, 5.0, 6.0},    // probe1
        {7.0, 8.0, 9.0},    // probe2
        {10.0, 11.0, 12.0}, // probe3
        {13.0, 14.0, 15.0}, // probe4
        {16.0, std::nan(""), 18.0}  // probe5
    };
    Anno anno = {
        {"probe0", "gene2"},
        {"probe1", "gene2"},
        {"probe2", "gene0"},
        {"probe3", "gene1"},
        {"probe4", "gene2"},
        {"probe5", "gene1"}
    };
    std::unordered_map<std::string, ExprPtrs> gene_probe_expr_ptrs;
    GeneExprHelper::Map(anno, probes, probe_exprs, gene_probe_expr_ptrs);
    Ids genes;
    Exprs gene_exprs;
    std::string method = "mean";
    GeneExprHelper::Aggregate(gene_probe_expr_ptrs, method, genes, gene_exprs);

    // compare genes
    QCOMPARE(genes.size(), 3);
    QCOMPARE(genes[0], "gene0");
    QCOMPARE(genes[1], "gene1");
    QCOMPARE(genes[2], "gene2");

    // compare gene0
    Expr gene0_expr = gene_exprs[0];
    QCOMPARE(gene0_expr.size(), 3);
    QCOMPARE(gene0_expr[0], 7.0);
    QCOMPARE(gene0_expr[1], 8.0);
    QCOMPARE(gene0_expr[2], 9.0);

    // compare gene1
    Expr gene1_expr = gene_exprs[1];
    QCOMPARE(gene1_expr.size(), 3);
    QCOMPARE(gene1_expr[0], 13.0);
    QCOMPARE(gene1_expr[1], std::nan(""));
    QCOMPARE(gene1_expr[2], 15.0);

    // compare gene2
    Expr gene2_expr = gene_exprs[2];
    QCOMPARE(gene2_expr.size(), 3);
    QCOMPARE(gene2_expr[0], 6.0);
    QCOMPARE(gene2_expr[1], 7.0);
    QCOMPARE(gene2_expr[2], 8.0);
}

void TestGeneExprHelper::test_Aggregate_Median() {
    Ids probes = {"probe0", "probe1", "probe2", "probe3", "probe4", "probe5"};
    Exprs probe_exprs = {
        {1.0, 2.0, 3.0},    // probe0
        {4.0, 5.0, 6.0},    // probe1
        {7.0, 8.0, 9.0},    // probe2
        {10.0, 11.0, 12.0}, // probe3
        {13.0, 14.0, 15.0}, // probe4
        {16.0, std::nan(""), 18.0}  // probe5
    };
    Anno anno = {
        {"probe0", "gene2"},
        {"probe1", "gene2"},
        {"probe2", "gene0"},
        {"probe3", "gene1"},
        {"probe4", "gene2"},
        {"probe5", "gene1"}
    };
    std::unordered_map<std::string, ExprPtrs> gene_probe_expr_ptrs;
    GeneExprHelper::Map(anno, probes, probe_exprs, gene_probe_expr_ptrs);
    Ids genes;
    Exprs gene_exprs;
    std::string method = "median";
    GeneExprHelper::Aggregate(gene_probe_expr_ptrs, method, genes, gene_exprs);

    // compare genes
    QCOMPARE(genes.size(), 3);
    QCOMPARE(genes[0], "gene0");
    QCOMPARE(genes[1], "gene1");
    QCOMPARE(genes[2], "gene2");

    // compare gene0
    Expr gene0_expr = gene_exprs[0];
    QCOMPARE(gene0_expr.size(), 3);
    QCOMPARE(gene0_expr[0], 7.0);
    QCOMPARE(gene0_expr[1], 8.0);
    QCOMPARE(gene0_expr[2], 9.0);

    // compare gene1
    Expr gene1_expr = gene_exprs[1];
    QCOMPARE(gene1_expr.size(), 3);
    QCOMPARE(gene1_expr[0], 13.0);
    QCOMPARE(gene1_expr[1], std::nan(""));
    QCOMPARE(gene1_expr[2], 15.0);

    // compare gene2
    Expr gene2_expr = gene_exprs[2];
    QCOMPARE(gene2_expr.size(), 3);
    QCOMPARE(gene2_expr[0], 4.0);
    QCOMPARE(gene2_expr[1], 5.0);
    QCOMPARE(gene2_expr[2], 6.0);
}

void TestGeneExprHelper::test_Aggregate_Max() {
    Ids probes = {"probe0", "probe1", "probe2", "probe3", "probe4", "probe5"};
    Exprs probe_exprs = {
        {1.0, 2.0, 3.0},    // probe0
        {4.0, 5.0, 6.0},    // probe1
        {7.0, 8.0, 9.0},    // probe2
        {10.0, 11.0, 12.0}, // probe3
        {13.0, 14.0, 15.0}, // probe4
        {16.0, std::nan(""), 18.0}  // probe5
    };
    Anno anno = {
        {"probe0", "gene2"},
        {"probe1", "gene2"},
        {"probe2", "gene0"},
        {"probe3", "gene1"},
        {"probe4", "gene2"},
        {"probe5", "gene1"}
    };
    std::unordered_map<std::string, ExprPtrs> gene_probe_expr_ptrs;
    GeneExprHelper::Map(anno, probes, probe_exprs, gene_probe_expr_ptrs);
    Ids genes;
    Exprs gene_exprs;
    std::string method = "max";
    GeneExprHelper::Aggregate(gene_probe_expr_ptrs, method, genes, gene_exprs);

    // compare genes
    QCOMPARE(genes.size(), 3);
    QCOMPARE(genes[0], "gene0");
    QCOMPARE(genes[1], "gene1");
    QCOMPARE(genes[2], "gene2");

    // compare gene0
    Expr gene0_expr = gene_exprs[0];
    QCOMPARE(gene0_expr.size(), 3);
    QCOMPARE(gene0_expr[0], 7.0);
    QCOMPARE(gene0_expr[1], 8.0);
    QCOMPARE(gene0_expr[2], 9.0);

    // compare gene1
    Expr gene1_expr = gene_exprs[1];
    QCOMPARE(gene1_expr.size(), 3);
    QCOMPARE(gene1_expr[0], 16.0);
    QCOMPARE(gene1_expr[1], std::nan(""));
    QCOMPARE(gene1_expr[2], 18.0);

    // compare gene2
    Expr gene2_expr = gene_exprs[2];
    QCOMPARE(gene2_expr.size(), 3);
    QCOMPARE(gene2_expr[0], 13.0);
    QCOMPARE(gene2_expr[1], 14.0);
    QCOMPARE(gene2_expr[2], 15.0);
}

void TestGeneExprHelper::test_Aggregate_Min() {
    Ids probes = {"probe0", "probe1", "probe2", "probe3", "probe4", "probe5"};
    Exprs probe_exprs = {
        {1.0, 2.0, 3.0},    // probe0
        {4.0, 5.0, 6.0},    // probe1
        {7.0, 8.0, 9.0},    // probe2
        {10.0, 11.0, 12.0}, // probe3
        {13.0, 14.0, 15.0}, // probe4
        {16.0, std::nan(""), 18.0}  // probe5
    };
    Anno anno = {
        {"probe0", "gene2"},
        {"probe1", "gene2"},
        {"probe2", "gene0"},
        {"probe3", "gene1"},
        {"probe4", "gene2"},
        {"probe5", "gene1"}
    };
    std::unordered_map<std::string, ExprPtrs> gene_probe_expr_ptrs;
    GeneExprHelper::Map(anno, probes, probe_exprs, gene_probe_expr_ptrs);
    Ids genes;
    Exprs gene_exprs;
    std::string method = "min";
    GeneExprHelper::Aggregate(gene_probe_expr_ptrs, method, genes, gene_exprs);

    // compare genes
    QCOMPARE(genes.size(), 3);
    QCOMPARE(genes[0], "gene0");
    QCOMPARE(genes[1], "gene1");
    QCOMPARE(genes[2], "gene2");

    // compare gene0
    Expr gene0_expr = gene_exprs[0];
    QCOMPARE(gene0_expr.size(), 3);
    QCOMPARE(gene0_expr[0], 7.0);
    QCOMPARE(gene0_expr[1], 8.0);
    QCOMPARE(gene0_expr[2], 9.0);

    // compare gene1
    Expr gene1_expr = gene_exprs[1];
    QCOMPARE(gene1_expr.size(), 3);
    QCOMPARE(gene1_expr[0], 10.0);
    QCOMPARE(gene1_expr[1], std::nan(""));
    QCOMPARE(gene1_expr[2], 12.0);

    // compare gene2
    Expr gene2_expr = gene_exprs[2];
    QCOMPARE(gene2_expr.size(), 3);
    QCOMPARE(gene2_expr[0], 1.0);
    QCOMPARE(gene2_expr[1], 2.0);
    QCOMPARE(gene2_expr[2], 3.0);
}
