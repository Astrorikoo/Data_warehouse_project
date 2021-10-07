
SELECT GROUP_CONCAT(P.ProductName,",")AS NUmbe

FROM 'Order' O
	LEFT JOIN OrderDetail OD ON OD.OrderId = O.Id  
	LEFT JOIN Customer C ON C.Id = O.CustomerId 
	LEFT JOIN Product P ON P.Id = OD.ProductId
	
WHERE C.CompanyName = 'Queen Cozinha'
AND SUBSTR(O.OrderDate,1,10)='2014-12-25'
ORDER BY P.Id;





