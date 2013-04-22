import module namespace xqxq = 'http://www.zorba-xquery.com/modules/xqxq';

variable $queryID := xqxq:prepare-main-module('declare variable $ext as xs:integer external; $ext');
variable $queryID2 := xqxq:prepare-main-module('declare variable $ext as xs:string external; $ext');
variable $queryID3 := xqxq:prepare-main-module('declare variable $ext as xs:dateTime external; $ext');
variable $queryID4 := xqxq:prepare-main-module('declare variable $ext as object() external; $ext');
variable $queryID5 := xqxq:prepare-main-module('declare variable $ext as element() external; $ext');

xqxq:variable-type($queryID, xs:QName('ext')),
xqxq:variable-type($queryID2, xs:QName('ext')),
xqxq:variable-type($queryID3, xs:QName('ext')),
xqxq:variable-type($queryID4, xs:QName('ext')),
xqxq:variable-type($queryID5, xs:QName('ext'))