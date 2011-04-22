import module namespace datetime = "http://www.zorba-xquery.com/modules/datetime";

declare %sequential function local:index()
{ 
  declare $table :=
      for $i in 1 to 10000
      return
          block 
          {
            declare $local:datetime;
            declare $local:date;
            declare $local:time;
            set $local:datetime := datetime:current-dateTime();
            set $local:date     := datetime:current-date();
            set $local:time     := datetime:current-time();
            <tr>
              <td> { $local:datetime } </td>
              <td> { $local:date } </td>
              <td> { $local:time } </td>                                                                               
            </tr>;
          };

  let $wrapped := <table> { $table } </table>
  return
      <html>
        <body>
          <b> Different DateTimes: { count(fn:distinct-values($wrapped//tr/td[1])) gt 1 } </b><br/>
          <b> Different Dates: { count(fn:distinct-values($wrapped//tr/td[2])) gt 1 } </b><br/>
          <b> Different Times: { count(fn:distinct-values($wrapped//tr/td[3])) gt 1 } </b><br/>
        </body>
      </html>;
};


local:index()
