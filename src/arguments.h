
#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include "commander/commander.h"

/** Parses the command line arguments.
 *
 *  This is a thin interface between nFlood and
 *  the `commander` cli argument parser.
 *
 *  Homepage: https://github.com/clibs/commander
 */
void arguments_parse(int argc, char* argv[]);

#endif /* ARGUMENTS_H */

