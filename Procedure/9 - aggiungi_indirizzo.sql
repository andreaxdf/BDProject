-- Aggiunta di un nuovo indirizzo per un fornitore.

CREATE PROCEDURE `aggiungi_indirizzo` (
	in var_codiceFornitore INT,
	in var_città VARCHAR(45),
	in var_via VARCHAR(45))
BEGIN

	declare exit handler for sqlexception
    begin
        rollback; 
        resignal;
    end;

	set transaction isolation level serializable;
    start transaction;
		insert into `Indirizzo` (`fornitore`, `via`, `città`)
		values (var_codiceFornitore, var_via, var_città);
	commit;
END