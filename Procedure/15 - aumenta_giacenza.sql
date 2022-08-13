-- Aumento giacenza di una specie.

CREATE PROCEDURE `aumenta_giacenza` (
	in var_codiceSpecie VARCHAR(10), 
	in var_quantità INT)
BEGIN

	declare exit handler for sqlexception
    begin
        rollback; 
        resignal;
    end;

	set transaction isolation level serializable;
    start transaction;
		update `Specie` 
			set `giacenza` = `giacenza` + var_quantità
			where `codiceSpecie` = var_codiceSpecie;
	commit;
END