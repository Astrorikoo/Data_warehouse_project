SELECT Id , ShipCountry , 
CASE
	WHEN ShipCountry in( 'USA','Mexico','Canada') THEN 'NorthAmerica'
	ELSE 'OtherPlace'
END AS Region FROM 'Order'

WHERE Id>=15445  
ORDER BY Id limit 20; 
