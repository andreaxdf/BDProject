--Op.4: Inserimento nuovo cliente privato.

CREATE PROCEDURE `aggiungi_cliente_privato` (
	in var_tipo CHAR, 
	in var_indirizzoFatturazione VARCHAR(60), 
	in var_indirizzoResidenza VARCHAR(60), 
	in var_nome VARCHAR(30), in var_cf VARCHAR(16), 
	in var_cognome VARCHAR(30))
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
		
		DECLARE @Pos int, @OneCode varchar(10) -- adjust the single element length to fit your needs
 
		SET @CodeList = LTRIM(RTRIM(@CodeList))+ ','
		SET @Pos = CHARINDEX(',', @CodeList, 1)

		IF REPLACE(@CodeList, ',', '') <> ''
		BEGIN
		    WHILE @Pos > 0
				BEGIN
					SET @OneCode = LTRIM(RTRIM(LEFT(@CodeList, @Pos - 1)))
					IF @OneCode <> ''
					   BEGIN
						  INSERT INTO @ParsedList (OneCode) VALUES (@OneCode)
					   END
					   SET @CodeList = RIGHT(@CodeList, LEN(@CodeList) - @Pos)
					   SET @Pos = CHARINDEX(',', @CodeList, 1)
				END
		END
		
		insert into `Cliente` (`contattabile`, `tipo`, `indirizzoFatturazione`, `indirizzoResidenza`, `nome`, `codiceFiscale`, `cognome`)
		values (var_codiceContattabile, var_tipo, var_indirizzoFatturazione, var_indirizzoResidenza, var_nome, var_cf, var_cognome);
	commit;
END