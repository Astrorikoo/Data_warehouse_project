SELECT Delay.CompanyName , 
ROUND(CAST(Delay.DelayDay AS FLOAT)*100/CAST(Total.TotalDay AS FLOAT),2) AS PerLate
FROM 
	(SELECT S.Id,S.CompanyName,COUNT(*) As DelayDay
	FROM 'Order' O, Shipper S
	WHERE S.Id = O.ShipVia AND O.ShippedDate > O.RequiredDate
	GROUP BY S.Id) Delay
	,
	(SELECT S.Id,S.CompanyName,COUNT(*) As TotalDay
	FROM 'Order' O, Shipper S
	WHERE S.Id = O.ShipVia 
	GROUP BY S.Id) Total
WHERE Delay.Id = Total.Id
ORDER BY Delay.CompanyName DESC;
