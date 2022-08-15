-- Inserimento nuovo cliente privato.

CREATE PROCEDURE `aggiungi_cliente_privato` (
	in var_tipo VARCHAR(1), 
	in var_indirizzoFatturazione VARCHAR(100), 
	in var_indirizzoResidenza VARCHAR(100), 
	in var_nome VARCHAR(45), 
	in var_cf VARCHAR(16), 
	in var_cognome VARCHAR(45))
BEGIN

	declare var_codiceContattabile varchar(45);

	declare exit handler for sqlexception
    begin
        rollback;
        resignal;
    end;

	set transaction isolation level serializable;
    start transaction;
		insert into `Contattabile`;
		var_codiceContattabile = last_insert_id();
		
		insert into `Cliente` (`contattabile`, `tipo`, `indirizzoFatturazione`, `indirizzoResidenza`, `nome`, `codiceFiscale`, `cognome`)
		values (var_codiceContattabile, var_tipo, var_indirizzoFatturazione, var_indirizzoResidenza, var_nome, var_cf, var_cognome);
	commit;
END