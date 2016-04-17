﻿/*
 * [The "BSD license"]
 *  Copyright (c) 2016 Mike Lischke
 *  Copyright (c) 2013 Terence Parr
 *  Copyright (c) 2013 Dan McLaughlin
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 *  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 *  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 *  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ATNType.h"
#include "LexerATNSimulator.h"
#include "DFA.h"
#include "EmptyPredictionContext.h"
#include "Exceptions.h"

#include "LexerInterpreter.h"

using namespace org::antlr::v4::runtime;

LexerInterpreter::LexerInterpreter(const std::wstring &grammarFileName, const std::vector<std::wstring> &tokenNames,
  const std::vector<std::wstring> &ruleNames, const std::vector<std::wstring> &modeNames, const atn::ATN &atn,
  CharStream *input)
  : Lexer(input), grammarFileName(grammarFileName), _tokenNames(tokenNames), _ruleNames(ruleNames), _modeNames(modeNames),
    _atn(atn), _sharedContextCache(new atn::PredictionContextCache()) {

  if (_atn.grammarType != atn::ATNType::LEXER) {
    throw IllegalArgumentException("The ATN must be a lexer ATN.");
  }

  for (size_t i = 0; i < (size_t)atn.getNumberOfDecisions(); ++i) {
    _decisionToDFA.push_back(dfa::DFA(_atn.getDecisionState((int)i), (int)i));
  }
  _interpreter = new atn::LexerATNSimulator(_atn, _decisionToDFA, _sharedContextCache);
}

LexerInterpreter::~LexerInterpreter()
{
  delete _interpreter;
}

const atn::ATN& LexerInterpreter::getATN() const {
  return _atn;
}

std::wstring LexerInterpreter::getGrammarFileName() const {
  return grammarFileName;
}

const std::vector<std::wstring>& LexerInterpreter::getTokenNames() const {
  return _tokenNames;
}

const std::vector<std::wstring>& LexerInterpreter::getRuleNames() const {
  return _ruleNames;
}

const std::vector<std::wstring>& LexerInterpreter::getModeNames() const {
  return _modeNames;
}
