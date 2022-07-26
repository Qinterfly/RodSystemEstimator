/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of aliases used in the application
 */

#ifndef APPALIASES_H
#define APPALIASES_H

#include <vector>
#include <memory>

namespace KLP
{
class Result;
}

namespace RSE
{

using Results = std::vector<std::shared_ptr<KLP::Result>>;

}

#endif // APPALIASES_H
