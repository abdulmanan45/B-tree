#include"B-tree.h"

int main() {

btree<string> t(5);
t.insert("AC", 1);
t.insert("NCCA", 2);
t.insert("GAA", 3);
t.insert("AC", 4);
t.insert("HFC", 5);
t.insert("EFS", 6);
t.insert("KDD", 7);
t.insert("QXXX", 8);
t.insert("MEV", 9);
t.insert("FAW", 10);
t.insert("WVGS", 11);
t.insert("LSGJ", 12);
t.insert("TATR", 13);
t.insert("ZDHJ", 14);
t.insert("PSFT", 15);
t.insert("RAA", 16);
t.insert("XSS", 17);
t.insert("YGG", 18);
t.insert("DDD", 19);
t.insert("SSG", 20);
t.insert("ACb", 1);
t.insert("NCBCA", 2);
t.insert("GABA", 3);
t.insert("ACD", 4);
t.insert("HFC", 5);
t.insert("ECFS", 6);
t.insert("KDSD", 7);
t.insert("QXXXX", 8);
t.insert("MEAV", 9);
t.insert("FSAW", 10);
t.insert("WVSGS", 11);
t.insert("LSKGJ", 12);
t.insert("TLFATR", 13);
t.insert("ZDOHJ", 14);
t.insert("PSVFT", 15);
t.insert("RVAA", 16);
t.insert("XSSS", 17);
t.insert("WYGG", 18);
t.insert("DQDD", 19);
t.insert("SXSG", 20);
  
 t.traverse(t.root);
}
