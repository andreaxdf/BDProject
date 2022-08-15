-- Modifica il contatto preferito di un contattabile.

CREATE PROCEDURE `modifica_contatto_preferito` (
	in var_contattabile INT,
	in var_contatto VARCHAR(45))
BEGIN

	declare exit handler for sqlexception
    begin
        rollback; 
        resignal;
    end;

	set transaction isolation level serializable;
    start transaction;
		update `Contattabile` 
			set `contattoPreferito` = var_contatto
			where `codice` = var_contattabile;
	commit;
END