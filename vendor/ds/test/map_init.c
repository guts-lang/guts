/*
 * MIT License
 *
 * Copyright (c) 2016-2018 Abel Lucas <www.github.com/uael>
 * Copyright (c) 2016-2018 Guts <www.github.com/guts-lang>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <string.h>

#include "test.h"

#include "ds/map.h"
#include "ds/set.h"

#define KEYS 1000

static __const char *__keys[KEYS] = {
	"nJbJ11pkdqK", "Y2ADcmgcKc3", "xrhTl4JfsZM", "3nyh0xJRydP", "lh3WTJfJXwl",
	"AFryQZnP0Bs", "M1hctGEfuF2", "i4dDLUEWwGV", "INLZwcQ6l8O", "aCGFtFtN4Be",
	"VkwcR6ClI6t", "kSFVb0TjMz8", "jTioS0neIOQ", "44hmlknlKLE", "zjPI4dEx3w2",
	"TUHpNW1L3vi", "5etMbgX4j4N", "iFncyScxsyH", "GcZglXwmavs", "QAoBuIYbNMB",
	"AVsbCxivNCZ", "V0lc8rNzvwR", "HAqAFeUHLj5", "l3qEkQFPeVR", "rtbKVVHjnXu",
	"dFFvKpXseCs", "SQlmKkpIPa1", "7XyNllYtBOb", "UnXDPjfsXfZ", "ElDbHuFN5al",
	"a4egW3CtlXE", "1mei2PXMaOE", "3sXNzE1afJH", "XBbMaxX9zAB", "i5OllXkxzB7",
	"2peAWCbvfny", "iQXd7KcUjq8", "4GjOKGNRLkN", "SgGqtCnvdkc", "xqHneh7yRO9",
	"pUXduEDrMLC", "jelMmNsCzfh", "yMm0fs1DyEx", "gzjAFQyCJCz", "4goYnftougl",
	"kqw5oECqnRJ", "bf5KgxMCWFz", "fp9D2G15moA", "fbbUg3jccUL", "JsYf2quvsiG",
	"AdJLoUFtTtJ", "AQfsMgynW4N", "XVsMeIGAPjx", "LrB81WMkOTh", "IzNru4F04Ew",
	"peKvwk3Wyf8", "j9tQpOcTVZ8", "2SmnyOda2zr", "GFKkx2CUF9I", "QfZrthvumJo",
	"ogvOYnxu7lO", "Kb3Bhojo0SC", "3CzBK9sONte", "MMPudWA2Tfb", "v7funhXGHme",
	"ESCbW3rV6Ru", "6VN0PdJBMVT", "rwcMc16DoXB", "GjoR7xZFLeq", "cr3erLIgy9C",
	"EKG6kZZPg0F", "e2fd7MJ1CKW", "cfVGuYmyWkq", "xUNy49WPAe2", "wuIFaloVqBX",
	"yvwz1AWIkPi", "wPlmvNvlbNQ", "LI9jqPMwiD9", "nGJN4Si6lzy", "vPYB6lL8SGa",
	"pO52iCu9MvC", "9JxWs4tNIlo", "DQuQgajhU0A", "Lcnpd9vDdr8", "Z5ipweYkDGs",
	"CnZH9Et8cb5", "BPV6F2QirSG", "AQiqtnHyYqk", "wOOXupRKZg2", "iDVJjAZm5Wd",
	"NtXC6xXtsbo", "rrHPxhgP4e0", "5C05hBMgLoG", "O0ENcsbWxdu", "nD7FlL06Ehj",
	"IbBiKuywZRb", "jJtwEfPGeYC", "UXK5pv1fJS4", "LgTHnwp5k56", "bmLPGhEyPPG",
	"vcs8G9mp5Ai", "MsKbjjRwU1L", "TAPt22MLYjZ", "MqEG3IzK5tL", "EQOXO1cgd51",
	"pJ123wduhrw", "gkGGQoDTqzz", "qPSIgQUOTTW", "lN2pp4svudA", "VMmPnD0LTt1",
	"eOvGwtMK02S", "KnOxqMg8BDE", "TdMyjekXzv1", "xW0N8L7ZAqF", "pCQHEVs83Qh",
	"t5nWT8wbbek", "MYNRyMWcyLF", "FrLzFe5SpNo", "YwKdbZeLDWR", "DJfG4THfW0Z",
	"hUdXnGyJ01b", "u23DByoRJtq", "JfHTWgN1nt3", "puSNqSgN5z3", "0RefuhGtp69",
	"eDr9KpCKLmM", "fuMASw5elxp", "oyt0boP3pFN", "VexLI9soJLk", "JY4hhRe90dr",
	"89OCPYJ5Axh", "cllu2WdYH17", "Ea8VsD7SmMG", "kxPOKqi9qDg", "yhTABSJ4wPn",
	"I35ZUdQ6Br1", "lIyj6RxEjfF", "pYXI1OxxsV4", "3BhmdmkAv5f", "WDth0K5m2Sj",
	"U9amplkPVMe", "DP6SpmiZ5eH", "64Et1Ptn0cJ", "lrBX0G5yeoa", "f3EFCOoM8Rx",
	"TMelwVqDJ84", "3RH4l3vH5q9", "oeESzrxbR0a", "kgPxiqcuQky", "UqSbLZvvoTf",
	"4Qok2NY15AV", "9Gwa0EYXCyp", "1HInmzDApZZ", "BbbANcsXfIE", "uJxWrp0p39C",
	"abCEPai94Ma", "Y4cvcFyqTBD", "MYxCjyH8ZrN", "0aAoX7Tdgck", "zYeHUjx7wgU",
	"6ZSiVQF2XKH", "Q9ZVdik2yDS", "fRJQFOsRNkt", "ZSarEC943AZ", "eoSfKdyD0Pi",
	"71hIBxY9k4j", "C6heS55JNk1", "uFIdMiOipdb", "Q2CigSvNa5d", "Drn38CkybTk",
	"An55SSMrmno", "oq2I4arUFeN", "gnlYrT1iQyR", "VWSyIY5XepK", "Agnl9xOtSgF",
	"nHp6gqdnuQG", "NMoW5SSfBgs", "DodcCPQkxbB", "vu1u6fJrLMg", "Np3CKBlc4MJ",
	"HzgDbdJgVJL", "xg9dseFwjHd", "PIjE7Bxm4j8", "7jo9JTRS33k", "UvoCDU7vrhO",
	"fO5NvKYn98a", "aRlXOYsQEqQ", "NRIpBywS7c2", "FVma5htXuaw", "6RW1c0DrYva",
	"zrtW7RbnJv9", "snLa8jc7HOu", "GxvEasXFTIJ", "kCgXz4nmy8P", "Vrg5A4BLai4",
	"g7naZyheegL", "rZgKWbIC5pD", "NL2O6weX0A4", "2v5f0ev1A3s", "AiGNiHnAJEo",
	"CmUOdij7YZ8", "nflVtoaeBYy", "0C0sZtpVEn9", "W3O4xWvxbIe", "sG1K4cp8Hg8",
	"NBDKjKQqsAK", "jiLvIErbRU0", "UrdbmhXRETe", "4LbtXbPZshQ", "uIspLzW6uZe",
	"kIh8iclcQuw", "9t3p7MrozBa", "ZzwsQyfsHJ2", "xKdhsXZEvfy", "mP7L8nNhlii",
	"3MyqmgseLdf", "Xt9417aXTne", "bEPdWSBfvNO", "ahma7QW6Nll", "7tw91YZyUq7",
	"vMzMyHW1kwd", "priDDuaNNAf", "tT5n6NHFRxp", "aAii97QB2xL", "cTBFrr5QAg6",
	"JK9OssPckRq", "wbs56AbticL", "7edJRB4BWie", "URmYVhYgVpJ", "bm5wKoEUCXg",
	"Ou04oxrX1fc", "Jc6ZJcQvYFv", "hBijfawy1RD", "adGlr8yGVe2", "GkHHz87i5pq",
	"25349kSXd1U", "CCyufKIts27", "U6cjR4kIvR7", "qZkH5LiewqP", "FwJ3RI1H4PZ",
	"7lUYsS76SCY", "wPC8A6yt1Hh", "ycazU1W1rza", "81gpluZ5SOt", "Kf6tprdeDzU",
	"fwmnQrBe8W7", "GJ9WJxfdOIA", "X33LhiY6lBu", "ZA5TvFmhwDe", "xes3njvGKrY",
	"ZGSQsW6faZR", "FOLCLo5j60H", "JeOCjuKRYeX", "1dzFbaclnqa", "XWJr7seTG8K",
	"UDx1SuwfL95", "4YhuKb0wvYq", "bmnqUTHFWcq", "nBv2k7ghffu", "MVKp2Nf3dPi",
	"L3MXVbcEZrU", "mGeM4Pnh3Y1", "4lIsWuBh8hQ", "JFPdn8GwV7C", "dkoOuAfZQy3",
	"1dUPOpcf6ok", "wNygDAZb7xk", "QuSCQzUpVHM", "68uqMY0Z7Z1", "PfUbaAaKwET",
	"c1wX6eEQYSt", "PLTE0jXRyN2", "MVVFGFsCwtU", "1JJwWVbhoLI", "4GeKVioheA9",
	"Y590mWPN2pQ", "sma2sgo7bvU", "Fa7kDEWqLfC", "7a2sayInMh2", "e2ibcki16ww",
	"oscbzV2wRTD", "i6HEIRSiDBp", "IPULAD0khTk", "lvqvZrdS8UE", "LMpQbf4RzLj",
	"Lo5JuCDa4Kc", "8XdlETk63Im", "ee2XtUWPvRK", "Clq1XXb2Q2o", "BiGOI0YsIpX",
	"b5eZPYeGe6e", "XzxDLtvFe4E", "eC3yHHeb1Vr", "h7J74vG9HBm", "BPHPstFDLIm",
	"63NMhID90AV", "bBYGGztIvbu", "a0Aq02zPQfF", "YYLhCrElLsu", "hiMbLUunhaK",
	"MO5CPncH8hB", "w4wgCLGL3KM", "iuevFT5TmrP", "qoYSKb2Q59M", "HTKOcbcA2Nc",
	"WnSUpoyfZ4a", "4HxHGc0oTfX", "fQXmQouX2Vt", "62zmq9wKfFm", "h6XfyxN60eX",
	"dwYnkcTwoYc", "NbbHe0X65JW", "g6opKyoPhfS", "Gtwy3PtBrYD", "KYUgYY6dA4Y",
	"LkhoCMW7IB4", "WMrWEpxJUmv", "GyEV8xkG4dc", "l6AZwUPxjh3", "F2sdIcgVtm0",
	"I1jSzA2DvCU", "AYsXuvvYu5Z", "8OcAaPAIVqh", "4aTkAn3m8xc", "p5ow8d4Q5Td",
	"jhj0jpxeTbo", "If6IujXcNz3", "W8k9i6urEFR", "6EiL6NPntT4", "Rnk2ops534d",
	"NCxwnemy7Yi", "z4D5Yy4t01n", "cYhhBNYHTyb", "BCrioMHaa1H", "BrIL3AlYGmd",
	"TcbMNipunrr", "AOmkT0Fb68o", "mXF3ihBLXqA", "JRLuFb72LUC", "KEQO8Bjysdx",
	"84eaDIQqxKZ", "cUrTHdDVD5B", "Zz0wX5nGiJA", "wNXoTJo36Gm", "YvUKibBoSY4",
	"T8KLvkb3Uwq", "krGo8xksdow", "lwoOdmDw9D1", "0BePNjEcoD5", "Qd1r9ICiRKO",
	"spGnZydfCC1", "ajl1HQMMByg", "0qBMzUoauQE", "EhxsaXh4658", "Kus7dq2jqHK",
	"qbI9mINOUv5", "mzEVyOigF6i", "i72dPrMvlTs", "Gt0POCaodgx", "jl1ZqaFgHPG",
	"MPB1qHPwGqv", "G3fFyhdIAlW", "ew81kOGQNin", "xl0M4Zmdne0", "9mc2WOxIfIs",
	"plX2ArCtyzu", "m04tvmVTUZ9", "cPcbDXfcwBO", "9tbGGmyyio3", "ZqtQsAIqchE",
	"AtO69AXoVqY", "J3vLrzsRKdf", "cwxl7PHEh1s", "ibL3ISX7qkC", "9fR4zBSW6Al",
	"MXN1Ck0dncb", "5AMobBjTBrI", "Hkm9Oay1hUp", "quPEfjaiOrZ", "a84xnYfRGZ1",
	"1SJYNXUwDVp", "CJkHJsa5r48", "CfeBCeRTGjA", "aLzjz9wjoJC", "Qr4dEyaHV56",
	"BNr9qNgFDYR", "XUopX3Ms9CO", "9P4Blj3AUHv", "7YevOGku588", "EkOj5HE5eyN",
	"oWrjU9CqlfJ", "VkgrBWaV7lh", "9zO3u1jtAal", "MAXEPf5Kdlf", "6xOCTNP7vms",
	"XUt35G2rron", "VHqBV4bG1m3", "maxkuNbLb2Y", "UZR1IvqKeTs", "Na7Fdh4NjXk",
	"VEnb6NwVhVX", "8SujL79yN37", "JUI1EeWH951", "SEk5C8TPlVM", "bt6LiK1vEEv",
	"4Lv6zDUG1CA", "PGzxBaBXuFx", "RpjddGkRt62", "0WTkheMPuXQ", "UGstSOE6ert",
	"F6jY76pkLCW", "osW3NBSeQuT", "9SIk3eFvJjn", "nUlLtcEEsoQ", "lACpO1JJ9JL",
	"Bvklxmw9HNG", "wKXH7VAKT1Y", "FCINtetlLsg", "kwg40K1EMQY", "Z92Pny5BnwI",
	"NtZXpxKplBx", "owphIDllt2L", "Wk8fOCezJE6", "TZaPBPzGUOO", "N4SLlSai2Bt",
	"S6gTA9wwzsK", "Yr667croIlh", "cA1EV4c9lvs", "ncA9iGSzNun", "pFZRdWPGKcz",
	"KYDCCefOhOw", "Gt5Nx38SHbw", "l5xQYZXWGQY", "YRfgL6jlSfX", "BdYUuViULNy",
	"6R3qQ7DwvmB", "300TDsGmnBS", "rDdJiYnAHZj", "S23UMDhIXSQ", "akylVqlgMFL",
	"umMYhGCFTYn", "66B2ZyLOoZm", "TLhqHvsjK4h", "Bqpc40m5pn6", "nYUsUTINg3q",
	"KmMeVnPiBeJ", "fItzTjRDStl", "JbmbE5ATLiO", "mprG65zey72", "RIl1bDR4j4m",
	"ptuq8V0KJEH", "ksXGfQ03yqB", "xnjD03oIWW0", "48D7gzIDiRg", "ve17Ur6ICSl",
	"wTvRxcxVIlL", "tu24PDqexZJ", "CjxRHH4x1r3", "s94nP9c3yTy", "MNlUTHYLdwj",
	"IfCbvhMILvS", "0Cpr3apqRVq", "VhTcU4Cyahu", "LxNKj1UarzB", "4H28xaCvCV6",
	"XWGdkY06Ji7", "HsLkURSPWlJ", "XYj53FhmOi5", "dcI7RpJCru3", "bhAbqktjyhc",
	"3Vmoq9Tdib4", "NtJ92lfuW9u", "0B94bmWJQkG", "kiuphCl4Mir", "D22S1fe3Zkn",
	"G2sIXr9Xext", "ItNHzaYkmOW", "k6XmLzYuTWT", "bhGJJhayMaH", "oRMHJhizKZv",
	"SYmzCz5bURh", "0OhCZSXRHhY", "3K9C6b3ctQv", "p0yPaJKia3G", "ANAdYYkpamw",
	"alCYxbMZSWk", "btR9QyKe8P5", "B4n4utOb6my", "4PHLs0cHfrI", "nEYCb45kkFn",
	"IQRBTdeM5Gy", "UA5UU6I8Avn", "S0jauitGwkN", "0BrytyszLbb", "7uvwweR9GoC",
	"cKUDPlv6uaE", "vHPBTpwE6w6", "lOQocSfdgUR", "6YdzUw9vyon", "tcG0aIozHCu",
	"lXNoNlPGz0I", "jioqPQUJ6L1", "ZolBTVvWEDI", "NUr13ee1trB", "WlIQbnyPvYV",
	"iGsbmaeRLlB", "17DcSi7ISIq", "fLFSyaJQInZ", "cyMC7v9tLpi", "znYU7W1qlvz",
	"OGgTpAnS4J5", "77s923Jtj2e", "Y54wbGMkA4a", "FTlD1uiz3aD", "xG8qlWBDB4K",
	"JEuYd6eQeu8", "G5bpSID6xzw", "jtvNqPXF7eW", "0ZzVgrfi7Hp", "aUPLcvr0puy",
	"O85yBYkjuRL", "W52qFHXygpU", "aTLQah9Viq2", "I6l3uAvjIkh", "Iz7Wr5DCdO5",
	"vfBxl9kSNqc", "yxJZsRnIVEb", "vYKxpxNa3h5", "plajyOyotBn", "1uBrRil9BLN",
	"4NtGbsJfYY2", "K2eMwT9F1Ow", "DQaPP7mVyqY", "5pfD2RzRpp5", "Nki6rRLngA4",
	"0KbwqWhCDbk", "wAPZ1KEaGeu", "8eahN590rW9", "D2RCGxFb0of", "pNTntm85WHY",
	"dEcs2eWXkst", "WQKWlviK7gT", "LM21dRZuTeW", "wATdZPfLWNi", "xskeKpfFj7S",
	"ttNam4r1Wyx", "6UmBVMOGt4Y", "lFXXf7QvSAy", "CKsWZxC0ojr", "J05uzTMuoeH",
	"naUuSLOBzK2", "5nZfpj3ujhh", "0dHuaxLWbHd", "IFqNKGAWEBJ", "fWot9GUqGGZ",
	"bwi2cdgkusl", "ASaBviZitAH", "i3BJAMZdmcp", "UMjaDCtRFLm", "cnqH8iRWtlU",
	"KIFwZFeq8wz", "0x7CxDTpiw4", "G9MpMzi9vVy", "YXXrIqxGFNF", "p1K8sTux2XX",
	"RnCIJwnhc3A", "66ypMFn95FX", "kTCCLLwPogQ", "bdjE3y8MH8k", "DZ8rhBNADVz",
	"vGB3Abatacg", "TmsQXpkUjdR", "6IotVUiB6Us", "52M7ntgyPMB", "hM4KVdhl5gH",
	"GGIB6a2cMlE", "XLLhWt9VXAh", "25GUM0QSiJR", "f1TsjbdQudl", "T8b1am3qtmm",
	"v7wm5JSCtwf", "7GVcRuvcjS6", "qn8DYDDDfSQ", "csdmZ2Ukgdf", "kI5LuYbVjVT",
	"tLvKi4uBA7V", "q4AjiAmFCyU", "lMtSgbCJXda", "56mC0uLYDLp", "JBs0AnFz93A",
	"yYEzHa8VyGs", "FkUsmJNRdA9", "ZNuAZFMqOrt", "AlVyfkZUJnq", "cJ74HwAwk4j",
	"Hv7Rnj0KslM", "PEcP60aQSVn", "7bJHNgcLvnv", "JPB8EUKiMoq", "9jmsS9PxUJY",
	"WkIhvKsGKTp", "NnXtQgLUc4k", "qNaaEdnnztQ", "qPuV4cKaJli", "jGGe3khZbri",
	"ahFiz8SC5sv", "8P1kDgumNZ9", "E0vSgYsmxLj", "Y1cP8Jci64r", "daoj0J8PUla",
	"5u9cuRHXoW4", "373e3QpFukJ", "tmjFsQOIlvx", "OaL2Wzpiyyt", "KmO34YuOd0J",
	"Uxu0EhMXXBC", "uV6gugWQG9V", "fzFwgcA4bSC", "ZIems18ASvf", "Vg3AGWMT08S",
	"ISuLDgO8g8n", "UPWcHTJJA9X", "xxZbqtc0wb2", "M0wa9RPJnl8", "etCT1Rqa2Eq",
	"74ffYDcWuGk", "SfZmTgHU5FF", "XNjvhHto7XB", "IPRyhEnsp1O", "FzUJy8v6rXr",
	"z9Jwllb1cbh", "wtmK1FlqbI6", "zQGKWix7FwW", "Mj2BFoEr83y", "qojfenlCLSD",
	"JFpbHUJcRKe", "aHeh3QL7SuZ", "nj151kgV93I", "jCTCw79QnQ4", "LlwLy2veams",
	"c5qnhxc9k4S", "5x7Ac9ARpJr", "nGmRF7M2m3N", "EW8Y50AZp6C", "A3xOqnI48DK",
	"qbQHZ11UEGx", "Z92Qpx6AY54", "fodpJgcQ7BN", "b9zUHC9Nv4M", "pCIN0ukzFt8",
	"Fcx4HFrtM17", "HCGBQjbnbvi", "ae1gBPYWy5l", "a9ZfiHxFNqA", "luEi7parioe",
	"eIDIqkZc8mI", "p9YA0cd6VkC", "7X6mKYt80M3", "ol9166c5DvO", "zbc2CjPfgo7",
	"cHWisAgwAvC", "KAlj5dej9AM", "pKWgPJF5sgj", "BfVySgWZBdx", "lTBEHsQ5YXD",
	"MjD92Nex4Sl", "aDpXYZccLn3", "GvlBKG5UJu6", "Q2B8XIoTPQ6", "tphIUqeSEuO",
	"DlCejD3No2M", "RbqpzKJzd5x", "hhhaKtMsr0j", "cGLewoDUwK0", "6P4qxjtEpkw",
	"QYlQNInAiPq", "xfVy8tBVvsC", "Y1SEgLapvY3", "mZLBbf1NidS", "zdGonhklq5l",
	"0pE3my8B0pS", "PBctRrTsz5R", "aVqxhRg3ULI", "Ppdayqnd7nq", "tk1fBX1uRwo",
	"YQo5RMMt6l3", "KpMOxlloCfH", "ONEHwPetDZs", "LCubBrCEc31", "9xerKxx2CuR",
	"01cViluxXtl", "yjwqWVcA8qi", "gPxJcuCPcJW", "TTZVkmG0oPs", "zhonvv1V6KN",
	"nrTRMjPA4hX", "cEAGlTukn8J", "JtkxoklEi2J", "txWqTNM2dSD", "rHHlAIiTss6",
	"z9GPVannZVt", "x7HDy90y2bR", "r3SA6MoTxLD", "sqClUcbgPcy", "7W2nyU8Mehr",
	"70oF6fHfLO8", "zZfhr8aezSl", "umLvEGg3lHp", "NDZLmlSXExq", "SteX55i5BKB",
	"G4E7sYjHrmZ", "EXbyEH4OaCn", "qAPOujDZshD", "P5EZZAvGqdH", "pjaQKosqRdk",
	"qXK5NX3x3Zo", "CS4wHWAFdIp", "AKKVJm9jMqv", "iWMKk9TvoBH", "lKgWA9v4JhN",
	"oP5CBaAoaSu", "1cNUUf4VzcS", "qemCAYE8EtZ", "UAyc7G6oOqU", "eGWoRid9QDF",
	"lkfbuyKg0ak", "C84V2CDNntK", "wZd65cV0ZxM", "pJsyxYPVSm2", "UFHgAOlqOC3",
	"iVqj98IHHWr", "SRc6VRThzro", "Wz4rgji4OWT", "i5j9BgOZqIt", "5wWIPCH7GOd",
	"hiH6BYQrTHf", "zyoEYwIioyJ", "38j9x4S1w8g", "a1fQAjWcd9C", "4t2hBYoYWdE",
	"3rjGXjV6pM2", "EKgIZD9nV4Q", "cEWyIv7QDHE", "zgbu86JT1jh", "sNUHp0UPfav",
	"SV54rnUk9FZ", "Du2a5DDiWh6", "HPDPXDsePhz", "drmN7u7eemF", "ozkqKEAzUtl",
	"QaCmCntj4Ol", "8blon4ZrKwl", "suJFstLFM4D", "qEc8peH5dEv", "aQYCuqdieac",
	"gopwwfhi6J7", "i9WbLIRoVI1", "31wqkrFY72h", "gxdXZm9Sbta", "tjL2uFlEjfo",
	"DCCEew5hlMC", "t8fZ3VDFzFi", "35G5MbuZ7ky", "pYkp2wlJbMT", "hOaNfUsoOkA",
	"8LnZmtkTgcV", "YYENv18pIWs", "bbTqdJmFrxP", "c3UvYHFc2Ru", "wVn0d6DYYgM",
	"61QwYgnKLkp", "Hze70r88rTd", "5BG0Xi1QgAS", "Itr5zwJa37n", "iXB5EoEDyKg",
	"XX0ugESQwyG", "MejCIAY2CM3", "pF5adXsDnP6", "TqNd5gxVUHr", "YGHRApttCq6",
	"KTSCFGaqSVb", "OQ28J8HhSL5", "lnXa3J2hnTY", "zx9G25L6jmc", "giJnhUuVHn1",
	"X3sX5qtyYky", "JiSUgXUg4kA", "b2DalSrHNEZ", "xXumWE6kG0i", "ROe7aJ2S6sX",
	"3TiBPqVyDOV", "Dhs0BLIeXjk", "XjImnv8xK8b", "1IQgBCqbkVv", "2XktUs3AabF",
	"iSkV5QA2nvg", "vTSHb8DJdNR", "3R8uixdDNmv", "lFJjZVBMVyy", "kwpOk2ncJQh",
	"e9u9im64UlL", "SNhtGHdv5sR", "TqcUIpqbxei", "mTaGLYEtUql", "JVonjVA7bRp",
	"TVORmZlTQqN", "Oq3LRuyDFDI", "fuDRIbQYgwj", "Nqb8TqiwBbW", "0TKb7PRqAJL",
	"qt6PZPdNhqk", "vNFaTY2HMEV", "7Te4HjborHK", "7wDjETnrXv6", "abLtlikA4mf",
	"izIbHlX5ugx", "KHWg4aIqS9U", "LPTWEnjP1w6", "pDBbdssXLO1", "Mt5DlNWTrjX",
	"q5WRBJE6hdx", "TVXMu6NdsrU", "alqGhKGGyy3", "XzZ7UJPmYwY", "zzRB5J877T4",
	"Po3b74fL4pH", "Svv0ZhVLReH", "bWyijC4qAkY", "M89rPlm1KLY", "mIzBqNgwLsu",
	"FhnMQ8Q7PoP", "9VkzoaBX0v1", "Gl7Yo3iQzf3", "7JHzUhP1uyV", "aCHGWkFNyIt",
	"H365clyHcV3", "JjpoVxrG9Dx", "1TWgWvF3yrH", "a5wdhTs8Iwm", "DyAGyOLZz1N",
	"fWap7TDCnYs", "k4mEWjuYVA5", "uuNQ6r7QWfa", "vkkACK4u56N", "4KgkKS7k6fD",
	"srTbeTreI7c", "UqL96SNGgVi", "MgNb9YLazvR", "VE4iB0Gw0k4", "FDWlBFpHbOK",
	"gJbZNpmVsWt", "USyi1GLxXYe", "p2k2FgThGpI", "jLBp7gSOPfX", "EqRoZHsNlZh",
	"TzNfY2GgoyX", "uOiWHomBNCr", "fE1gg9mu1VD", "Mvmk7KAtT6q", "YRu5FMj8wgO",
	"h6F28L0pH65", "3WZR3WyAqeA", "gS7uFOGUtMK", "MB9YwXnn8Gw", "AfF88yqDP60",
	"2vQHTjQRVif", "0FJ9oYUECBZ", "Csd6MDX0voM", "gzo1bkR4Rpi", "81y6odw3Bws",
	"kRdQM0D7kHs", "CBuNEtePuAj", "JUBeDXYqRHY", "Bzd67kZi4vz", "0LV40bav5EW",
	"YkuV637QwLT", "4DfwCUzDSBV", "0eYfWufdiZ5", "cfmkiT1MzwO", "fVa9kstwZa2",
	"bCnpBIp3G29", "E6luGug9O2O", "eA8HiJcU5v5", "N7QcX7lvBSK", "Ylk0nxeL2Jp",
	"8NjTv2O8u0L", "RPBeSDMvJAQ", "pzRjheTZt45", "LvoBOjtsvpg", "U4IEVoGmhsm",
	"0xZbhddBnUf", "ompCgYwVCkj", "NV27zQbpqLZ", "PZLZCpFF1lm", "euXVz6yYLb1",
	"HzGmLMSRkDa", "sPnwk23rVZs", "DSFGaunBE5g", "BpcvWAZpIeb", "ynYyNvKM3vN",
	"CiF00AF9WDE", "05L4gzXKF8T", "k6N6iWWqjDM", "omjgXITTZnD", "YgDofwJuug0",
	"orlTn3pqsbe", "sq8Oip2mIFy", "1g7IrT0M8Ld", "vctfXXKrZzW", "RahMaEMfqDM",
	"HV2fgpqFMJy", "3ZfVSckTUKf", "382mgzfdqcr", "6Q7STaeVF6v", "80Ns3tnydKS",
	"VbMh0ZITpNV", "NV8Uad22NSn", "cOsiQNPgl7i", "7fKlteUoYlZ", "SjgDPSCChLZ",
	"oU6K7jR0Wrc", "8ATK1mgSymF", "itLRrF9STYR", "qriQ2mtPX7m", "jcnfwtC0nFc",
	"QwK8qmeCHq2", "n9AMRhApTpC", "1yHlSZWQA10", "gvm86nSQwqF", "4pMIcViUbvr",
	"KEZ23YcJXF1", "KezK0tONYp7", "jmxwR8OY0uE", "7xSsfFj4vUi", "YGhrlBEcMjg",
	"BuMPIiFHMB5", "OGwG172m8ja", "vwrO05y9HS6", "jB24ixbqrIZ", "aAPRh50h8Fy",
	"Y2HBtAPLB6A", "7yyJRotYDaN", "gHrspdSDMvh", "CrY08PkYXPn", "eGSzrZbkFPL",
	"3U49kfYMHRS", "zZod6qFXBci", "o3IYL3j5rqF", "NXGJ3gm8f2O", "OdcH5VVWzbQ",
	"qtTpBw6mEc1", "U4gj8a1X7Es", "ka97GO1o7iF", "KMtK9Ks89o2", "jyxUhEhAA0R",
	"nFBhrULIvuT", "rn6zJhoEcRS", "IrJ1x1DRbdp", "DpFsJGW5Tno", "DscTJog9yXx",
	"Jg4yglWeLdO", "9zIFFY7E8uE", "CsNfogkN11g", "ByHblffpaCv", "RPLUQtRlRPW",
	"eBx2y3bfstU", "gx4wXm9ihwP", "KvLZvSBo02A", "hvQUyTyJWTK", "GSkV58e3FEr",
	"dzNHqfeIfDP", "laK3ggZpzKO", "p55A9H2liRa", "BSEoJbBQ9AS", "C5ma34dbMlg",
	"jpXUgXOgjnY", "V6cc8lb7BSg", "aDJP2vmZIyy", "kP6Eo87GOUV", "XhgpQbTstY4",
	"lS2nS5x7Lex", "1xfCxMqucO9", "7yZpdZiaFbR", "1QhYcsg4bMc", "GScv1CWh5II",
	"kRk4EkYgeO1", "JLLEFZs0mEr", "jDJfVV23Ulo", "tjuH5wGq1hm", "33rdNkgNgF2",
	"VrzS2kd0SED", "08vMjGkarwY", "29lDcnZyO9Y", "AnbEKakunrz", "DN9VTu3N56y",
	"YZEqlGgtYvB", "tEPsO4P1lVc", "1YFmj0Fhx7t", "OOJA27ECquO", "isyU9ZXLToi",
};

static mapof(char *, u32_t) __map;

int main(void)
{
	u32_t i, idx;
	htable_span_t e;

	mapinit(&__map, hash_str, eq_str);

	for (i = 0; i < KEYS; ++i)
		mapput(&__map, __keys[i], i);

	for (i = 0; i < KEYS; ++i) {
		ASSERT_TRUE(maphas(&__map, __keys[i]));

		idx = mapget(&__map, __keys[i]);
		ASSERT_NEQ(U32_MAX, idx);
		ASSERT_EQ(i, __map.entries[idx].val);
		ASSERT_STREQ(__map.entries[idx].key, __keys[i]);

		e.hash = hash_str((u8_t *)__keys[i]) << 2;
		ASSERT_EQ(e.hash, __map.spans[idx].hash);

		ASSERT_TRUE(mapdel(&__map, __keys[i]));
		ASSERT_FALSE(maphas(&__map, __keys[i]));
		ASSERT_FALSE(mapdel(&__map, __keys[i]));
	}

	free(__map.entries);
	free(__map.spans);
	return 0;
}
