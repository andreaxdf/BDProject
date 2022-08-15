-- Inserimento di una nuova specie di piante e del relativo prezzo.

CREATE PROCEDURE `aggiungi_specie` (
	in var_codiceSpecie VARCHAR(10), 
	in var_nomeLatino VARCHAR(45), 
	in var_nomeComune VARCHAR(45), 
	in var_giacenza INT, 
	in var_esotica VARCHAR(1), 
	in var_interno VARCHAR(1), 
	in var_valore DECIMAL(2))
BEGIN

	declare exit handler for sqlexception
    begin
        rollback; 
        resignal;
    end;

	set transaction isolation level serializable;
    start transaction;
		insert into `Specie` (`codiceSpecie`, `nomeLatino`, `nomeComune`, `giacenza`, `interno`, `esotica`, `dataPrezzoAttuale`)
		values (var_codiceSpecie, var_nomeLatino, var_nomeComune, var_giacenza, var_interno, var_esotica, GETDATE());
		
		insert into `Prezzo` (`specie`, `dataInizio`, `valore`)
		values (var_codiceSpecie, curdate(), var_valore)
	commit;
END