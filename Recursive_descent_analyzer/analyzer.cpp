/*************
 * ·ÖÎöÆ÷Ö÷º¯Êý
 * @Date 2019/12/4
 */

#ifndef VSDEBUG

#ifdef VSDEBUG
#include <cstdlib>
#endif

#include "lexical_analyzer.h"

using namespace std;

auto infile_name = "src.txt";
auto otpfile_name = "./results/lexical_analyzer_result.txt";

int main() {
    LEX_ANA lexical_analyzer(infile_name, otpfile_name);
    lexical_analyzer.read_file();
    lexical_analyzer.analyze();
    lexical_analyzer.output_result();


#ifdef VSDEBUG
    system("pause");
#endif
    return 0;
}


#endif // !DEBUG
