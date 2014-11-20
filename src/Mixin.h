/*
 * Copyright 2012 Bram van der Kroef
 *
 * This file is part of LESS CSS Compiler.
 *
 * LESS CSS Compiler is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LESS CSS Compiler is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LESS CSS Compiler.  If not, see <http://www.gnu.org/licenses/>. 
 *
 * Author: Bram van der Kroef <bram@vanderkroef.net>
 */

#ifndef __Mixin_h__
#define __Mixin_h__

#include "Selector.h"
#include "Stylesheet.h"
#include <list>
#include <map>

class LessStylesheet;
class LessRuleset;

class Mixin: public StylesheetStatement{
private:
  vector<TokenList*> arguments;
  map<string, TokenList*> namedArguments;

  LessStylesheet* lessStylesheet;
  void parseArguments(TokenListIterator* tli);

public:
  Selector* name;

  Mixin();
  Mixin(Selector* name);
  virtual ~Mixin() ;

  TokenList* getArgument(size_t i);
  size_t getArgumentCount();
  
  TokenList* getArgument(string name);

  bool insert(Stylesheet* s, Ruleset* ruleset, LessRuleset* parent);
  bool parse(Selector* selector);

  virtual void setStylesheet(LessStylesheet* stylesheet);
  LessStylesheet* getLessStylesheet();

  virtual void process(Stylesheet* s);
  virtual void write(CssWriter* writer) {};
};

#endif