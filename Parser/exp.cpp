Tree parseExpr()
{
    Tree left = parseTerm();
    while ( tk=get(), tk==tkPlus||tk==tkMinus )
        left = mkBinTree(tk,left,parseTerm());
    return left;
}
Tree parseTerm()
{
    Tree left = parseFactor();
    while ( tk=get(), tk==tkStar||tk==tkSlash )
        left = mkBinTree(tk,left,parseFactor());
    return left;
}
Tree parseFactor()
{
    Tree res;
    if ( tk=get(), tk==tkLParen )
    {
        res = parseExpr();
        get(); // skip ')'
    }
    else res = mkUnaryTree(parseId());
    return res;
}
