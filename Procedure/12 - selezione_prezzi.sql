-- Selezione di tutti i prezzi di una specie nel tempo.

CREATE PROCEDURE `seleziona_prezzi` (
	in var_codiceSpecie VARCHAR(10))
BEGIN

	declare exit handler for sqlexception
    begin
        rollback; 
        resignal;
    end;

	set transaction isolation level serializable;
    start transaction;
		select `valore`, `dataInizio`, `dataFine`
		from `Prezzo`
		where `specie` = var_codiceSpecie;
	commit;
END