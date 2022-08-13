-- Inserimento di un nuovo fornitore.

CREATE PROCEDURE `inserimento_fornitore` (
	in var_codiceFornitore VARCHAR(10), 
	in var_nome VARCHAR(45), 
	in var_codiceFiscale VARCHAR(16))
BEGIN

	declare exit handler for sqlexception
    begin
        rollback; 
        resignal;
    end;

	set transaction isolation level serializable;
    start transaction;
		insert into `Fornitore` (`codiceFornitore`, `nome`, `codiceFiscale`)
		values (var_codiceFornitore, var_nome, var_codiceFiscale);
	commit;
END