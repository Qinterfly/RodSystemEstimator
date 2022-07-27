/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of aliases used in KLP results
 */

#ifndef ALIASKLP_H
#define ALIASKLP_H

#include <vector>
#include <memory>

namespace KLP
{

class Result;
using Results = std::vector<std::shared_ptr<Result>>;

}

#endif // ALIASKLP_H
