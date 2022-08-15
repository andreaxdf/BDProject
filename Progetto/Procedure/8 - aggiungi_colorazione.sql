-- Aggiunta di una nuova colorazione ad una specie fiorita.

CREATE PROCEDURE `aggiungi_colorazione` (
	in var_codiceSpecie VARCHAR(10),
	in var_colore VARCHAR(45))
BEGIN

	declare exit handler for sqlexception
    begin
        rollback; 
        resignal;
    end;

	set transaction isolation level serializable;
    start transaction;
		insert into `Colorazione` (`specie`, `colore`)
		values (var_codiceSpecie, var_colore);
	commit;
END