/*****************************************************************************

    This file is part of Modelica C Compiler.

    Modelica C Compiler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Modelica C Compiler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Modelica C Compiler.  If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/

#include <util/debug.h>
#include <util/ast_visitors/replace_eq.h>
#include <boost/variant/apply_visitor.hpp>
#define apply(X) boost::apply_visitor(*this,X)

namespace Modelica {

    using namespace boost;
    replace_eq::replace_eq(Expression l, Expression r): look(l), rep(r), replace_exp(look,rep) {
      replace_exp.ignoreIndexes();
    };
    Equation replace_eq::operator()(Connect v) const {
      ERROR("Replace in connect equation not implemented\n");
      return v;
    };
    Equation replace_eq::operator()(Equality v) const {
      v.left_ref()=boost::apply_visitor(replace_exp, v.left_ref());
      v.right_ref()=boost::apply_visitor(replace_exp, v.right_ref());
      return v;
    };
    Equation replace_eq::operator()(IfEq v) const {
      ERROR("Replace in if equation not implemented\n");
      return v;
    }
    Equation replace_eq::operator()(CallEq v) const {
      ERROR("Replace in call equation not implemented\n");
      return v;
    };
    Equation replace_eq::operator()(ForEq v) const {
      foreach_(Index &i, v.range_ref().indexes_ref()) {
        if (i.exp()) {
          i.exp_ref()=boost::apply_visitor(replace_exp, i.exp_ref().get());
        }
      }
      foreach_(Equation &e, v.elements_ref()) {
        e = apply(e);
      }
      //ERROR("Replace in for equation not implemented\n");
      return v;
    };
    Equation replace_eq::operator()(WhenEq v) const {
      ERROR("Replace in when equation not implemented\n");
      return v;
    }
 
}
