
module.exports = {
  EoF: 256,
  DIGIT: 257
};

function isLayoutChar(ch) {
	switch(ch) {
		case ' ': case '\t': case '\n': return 1;
		default:                        return 0;
	}
};

// { type: integer, rep: string }
// using type instead of class because class is a restricted keyword, which is 
// a pain to program with.
module.exports.Token = {};

var input = "(1 * 3)"
var inputIndex = 0;
module.exports.getNextToken = function() {
	var ch;

	do {
    ch = input[inputIndex++];
    if (typeof ch === 'undefined') {
      module.exports.Token = { type: module.exports.EoF, rep: '#' };
      return;
    }
	} while(isLayoutChar(ch));
	
  module.exports.Token = {
    type: '0' <= ch && ch <= '9' ? module.exports.DIGIT : ch,
    rep: ch 
  };
};