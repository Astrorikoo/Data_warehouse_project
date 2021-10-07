WITH Total AS (
    WITH CTE AS (
        WITH CTE_BLONP AS (
            SELECT Id, OrderDate
            FROM "Order"
            WHERE "Order".CustomerId = 'BLONP'
            ORDER BY OrderDate
            LIMIT 10
        )
        SELECT *,
               lag(OrderDate) over ()           AS Tem,
               rank() over (ORDER BY OrderDate) AS Ranks
        FROM CTE_BLONP
    )

    SELECT Id, OrderDate,
           CASE
               WHEN Ranks=1 THEN OrderDate
               ELSE Tem
               END AS PreOrderDate
    FROM CTE
)

SELECT Id  , OrderDate  ,
       PreOrderDate  ,
       ROUND(CAST(julianday(OrderDate) - julianday(PreOrderDate) AS FLOAT), 2) AS DateApart 
FROM Total

ORDER BY OrderDate;
