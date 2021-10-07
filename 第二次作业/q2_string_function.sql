select distinct ShipName, SUBSTR(ShipName,1,INSTR(ShipName,'-')-1) as PsName
from 'Order'
where ShipName like '%-%' ORDER BY PsName;

