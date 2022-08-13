-- Modifica prezzo attuale di una specie

CREATE PROCEDURE `modifica_prezzo_specie` (
	in var_codiceSpecie VARCHAR(10), 
	in var_prezzo DECIMAL(2))
BEGIN

	declare exit handler for sqlexception
    begin
        rollback; 
        resignal;
    end;

	set transaction isolation level serializable;
    start transaction;
		declare var_date DATE;
		SET var_date = GETDATE();
		
		insert into `Prezzo` (`specie`, `dataInizio`, `valore`)
		values (var_codiceSpecie, var_date, var_prezzo);
		
		update `Specie` 
			set dataPrezzoAttuale = var_date
			where codiceSpecie = var_codiceSpecie;
	commit;
END