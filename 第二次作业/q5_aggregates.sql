SELECT CategoryName,Count(ProductName)AS NumberofP,ROUND(AVG(UnitPrice),2)AS AVGP,
MIN(UnitPrice)AS MINP,MAX(UnitPrice)AS MAXP,SUM(UnitsONOrder)AS TotalO
FROM Category C,Product P
WHERE C.Id = P.CategoryId
GROUP BY C.Id
HAVING COUNT(*) >10;
