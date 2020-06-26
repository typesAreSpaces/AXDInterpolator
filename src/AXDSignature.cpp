#include "AXDSignature.h"

AXDSignature::AXDSignature(z3::context & ctx):
    int_sort(    ctx.int_sort()), 
    index_sort(  ctx.int_sort()), 
    element_sort(ctx.uninterpreted_sort("ELEM")), 
    array_sort(  ctx.uninterpreted_sort("ARRAY")),

    diff(  ctx.function("diff",   array_sort, array_sort, index_sort)),
    diff_k(ctx.function("diff_k", index_sort, array_sort, array_sort, index_sort)),
    wr(    ctx.function("wr",     array_sort, index_sort, element_sort, array_sort)),
    rd(    ctx.function("rd",     array_sort, index_sort, element_sort))
{
}
