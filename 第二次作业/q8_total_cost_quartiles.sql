
SELECT  CompanyName,CustomerId,ROUND(totalexp,2)AS Result
FROM
  (SELECT IFNULL(CompanyName, "MISSING_NAME") AS CompanyName, CustomerId, totalexp, NTILE(4) OVER(ORDER BY CAST(totalexp AS float) )AS  quartiles 
   FROM 
      (SELECT O.CustomerId, SUM((UnitPrice*Quantity)) AS totalexp 
      	FROM "Order" O
      	LEFT OUTER JOIN OrderDetail ON O.Id = OrderDetail.OrderId 
      	GROUP BY O.CustomerId
      )
    	LEFT OUTER JOIN Customer C ON CustomerId = C.Id
   )

WHERE  quartiles=1
ORDER BY CAST(totalexp AS float);

