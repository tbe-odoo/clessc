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

#ifndef __LessParser_h__
#define __LessParser_h__

#include "../css/CssParser.h"
#include "../css/CssTokenizer.h"
#include "../stylesheet/Stylesheet.h"
#include "../lessstylesheet/LessRuleset.h"
#include "../lessstylesheet/LessStylesheet.h"
#include "../lessstylesheet/LessMediaQuery.h"
#include "../lessstylesheet/MediaQueryRuleset.h"

#include "../Token.h"
#include "../TokenList.h"

#include "LessTokenizer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <list>
  
/**
 * Extends the css spec with these parts:
 * * Variables
 *  at-rule: ATKEYWORD S* [ ':' value ';' S* | any* [ block | ';' S* ] ];
 *  
 * * Mixins
 *  ruleset: selector? '{' S* [declaration | selector]?
 *               [ ';' S* [declaration | selector]? ]* '}' S*;
 *  
 * * Parametric Mixins
 *  ruleset: [selector parameters?]? '{' S* declaration?
 *              [ ';' S* declaration? ]* '}' S*;
 * parameters: '(' parameter? [ ',' S* parameter ]* ')'
 * parameter: ATKEYWORD [ ':' S* value ]? S*
 * 
 * * Nested Rules
 *  ruleset: [selector parameters]? '{' S* r-statement? '}' S*;
 *  r-statement: [ declaration [';' S* r-statement ]?
 *                | ruleset r-statement? ]
 * * Operations
 *
 *  value: value-old [ [  '+' | '-' | '*' | '/' ] value-old ]*
 *  
 */
class LessParser: public CssParser {
public:
  static const unsigned int IMPORT_REFERENCE = 1,
    IMPORT_INLINE = 2,
    IMPORT_LESS = 4,
    IMPORT_CSS = 8,
    IMPORT_ONCE = 16,
    IMPORT_MULTIPLE = 32,
    IMPORT_OPTIONAL = 64;

  std::list<const char*>* includePaths;
  
  LessParser(CssTokenizer &tokenizer,
             std::list<const char*> &source_files):
    CssParser(tokenizer),
    sources(source_files),
    reference(false) {
  }
  LessParser(CssTokenizer &tokenizer,
             std::list<const char*> &source_files,
             bool isreference):
    CssParser(tokenizer),
    sources(source_files),
    reference(isreference) {
  }
  virtual ~LessParser () {
  }

  virtual void parseStylesheet(LessStylesheet &stylesheet);
  
protected:
  std::list<const char*> &sources;
  bool reference;

  /**
   * Skip comments only if they are LESS comments, not CSS comments.
   */ 
  virtual void skipWhitespace();
  
  /**
   * If an AtRule->getRule() starts with a COLON, add the variable to
   * variables and don't add it to the Stylesheet.
   * 
   */
  bool parseStatement(Stylesheet &stylesheet);
  
  /**
   * If the first token for the rule is a COLON, parse a
   * value. Otherwise parse the usual at-rule.
   */
  bool parseAtRuleOrVariable (LessStylesheet &stylesheet);

  bool parseAtRuleValue(TokenList &rule);

  bool parseVariable (TokenList &value);
  bool parseSelector(Selector &selector);
  bool parseSelectorVariable(Selector &selector);
  
  bool parseRuleset (LessStylesheet &stylesheet,
                     Selector &selector,
                     LessRuleset* parent = NULL);

  void parseRulesetStatements (LessStylesheet &stylesheet,
                               LessRuleset &ruleset);

  void parseMediaQueryRuleset(Token &mediatoken,
                              LessStylesheet &stylesheet,
                              LessRuleset &parent);

  bool parsePropertyVariable(Selector &selector);
  UnprocessedStatement* parseRulesetStatement (LessRuleset &parent);

  
  Declaration* parseDeclaration(TokenList &property, TokenList &value);

  bool parseVariable(std::string &keyword, TokenList &value);

  
  void parseList(std::list<TokenList*>* list, TokenList* tokens);

  bool parseImportStatement(TokenList &statement, LessStylesheet &stylesheet);
  unsigned int parseImportDirective(Token &t);
  bool importFile(Token uri, LessStylesheet &stylesheet, unsigned int directive);

  void parseLessMediaQuery(Token &mediatoken,
                           LessStylesheet &stylesheet);
private:
    
  TokenList* processValue(TokenList* value);
  
  std::list<TokenList*>* processArguments(TokenList* arguments);

  bool findFile(Token& uri, std::string& filename);
};

#endif
