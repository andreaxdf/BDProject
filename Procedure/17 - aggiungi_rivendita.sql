-- Inserimento di una nuova rivendita.

CREATE PROCEDURE `aggiungi_rivendita` (
	in var_tipo VARCHAR(1), 
	in var_indirizzoFatturazione VARCHAR(100), 
	in var_indirizzoResidenza VARCHAR(100), 
	in var_nomeRivendita VARCHAR(45), 
	in var_partitaIVA VARCHAR(11),
	in var_nomeReferente VARCHAR(45),
	in var_cognomeReferente VARCHAR(45),
	in var_contattoRivendita VARCHAR(45),
	in var_tipoContattoRivendita VARCHAR(1),
	in var_contattoReferente VARCHAR(45),
	in var_tipoContattoReferente VARCHAR(1))
BEGIN

	declare exit handler for sqlexception
    begin
        rollback; 
        resignal;
    end;

	set transaction isolation level serializable;
    start transaction;
		insert into `Contattabile`;
		var_codiceContattabileRivendita = last_insert_id();
		
		insert into `Contatto` (`contattabile`, `valore`, `tipo`)
		values (var_codiceContattabileRivendita, var_contattoRivendita, var_tipoContattoRivendita);
		
		insert into `Cliente` (`contattabile`, `tipo`, `indirizzoFatturazione`, `indirizzoResidenza`, `nome`)
		values (var_codiceContattabileRivendita, var_tipo, var_indirizzoFatturazione, var_indirizzoResidenza, var_nomeRivendita);

		---------- inserimento referente -----------

		insert into `Contattabile`;
		var_codiceContattabileReferente = last_insert_id();
		
		insert into `Contatto` (`contattabile`, `valore`, `tipo`)
		values (var_codiceContattabileReferente, var_contattoReferente, var_tipoContattoReferente);

		insert into `Referente` (`contattabile`, `nome`, `cognome`)
		values (var_codiceContattabileReferente, var_nomeReferente, var_cognomeReferente);

		--------------------------------------------

		insert into `Rivendita` (`cliente`, `partitaIVA`, `referente`)
		values (var_codiceContattabileRivendita, var_partitaIVA, var_codiceContattabileReferente);

	commit;
END