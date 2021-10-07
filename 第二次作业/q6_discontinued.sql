SELECT T.ProductName,T.CompanyName,T.ContactName
FROM
	(SELECT ProductName,C.CompanyName,C.ContactName,MIN(O.OrderDate)
	FROM OrderDetail OD, Product P, 'Order' O, Customer C
	WHERE OD.OrderId=O.Id
	AND OD.ProductId=P.Id
	AND O.CustomerId=C.Id
	AND P.Discontinued=1
	GROUP BY ProductName) T 
ORDER BY T.ProductName;
