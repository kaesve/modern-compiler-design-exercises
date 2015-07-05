var lex = require('./lex.js');


function parseOperator(operatorNode) {
	switch(lex.Token.type) {
    case '+': case '*': 
      operatorNode.operator = lex.Token.type;
      lex.getNextToken(); 
      return 1;
    default:            return 0;
  }
}

function parseExpression(expressionNode) {
  if (lex.Token.type === lex.DIGIT) {
    expressionNode.type = 'D';
    expressionNode.value = lex.Token.rep - '0';
    lex.getNextToken();
    return 1;
  }

  if (lex.Token.type === '(') {
    expressionNode.type = 'P';
    lex.getNextToken();
    if (!parseExpression(expressionNode.left = {})) {
      console.error('Missing left expression'); 
      return 0;
    }
    if (!parseOperator(expressionNode.oper = {})) {
      console.error('Missing operator');
    }
    if (!parseExpression(expressionNode.right = {})) {
      console.error('Missing right expression'); 
      return 0;
    }
    if (lex.Token.type !== ')') {
      console.error('Missing )'); 
      return 0;
    }
    lex.getNextToken();
    return 1;
  }
}

function parseProgram(rootNode) {
  lex.getNextToken();
  if (parseExpression(rootNode)) {
    if (lex.Token.type !== lex.EoF) {
      console.error('Garbarge after end of program');
      return 0;
    }
    return 1;
  }
  return 0;
}

module.exports = {
  parseProgram: parseProgram
}

var p = {}
console.log(parseProgram(p));
console.log(p);