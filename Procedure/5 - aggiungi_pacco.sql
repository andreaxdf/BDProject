--Op.5: Inserimento nuovo pacco.

CREATE PROCEDURE `aggiungi_pacco` (
	in var_ordine INT, 
	in var_numeroPacco INT,
	in var_listaSpecie VARCHAR(MAX),
	in var_listaQuantità VARCHAR(MAX))
BEGIN
	declare var_codiceContattabile varchar(45);

	declare exit handler for sqlexception
    begin
        rollback;
        resignal;
    end;

	set transaction isolation level serializable;
    start transaction;
		insert into `Pacco` (`numeroPacco`, `ordine`)
		values (var_numeroPacco, var_ordine);
		
		DECLARE var_posSpecie bigint, var_posQuantità bigint, var_specie varchar(10), var_quantità int
 
		SET var_listaSpecie = LTRIM(RTRIM(var_listaSpecie))+ ','
		SET var_listaQuantità = LTRIM(RTRIM(var_listaQuantità))+ ','
		SET var_posSpecie = CHARINDEX(',', var_listaSpecie, 1)
		SET var_posQuantità = CHARINDEX(',', var_posQuantità, 1)
 
		IF REPLACE(var_listaSpecie, ',', '') <> ''
		BEGIN
			WHILE var_posSpecie > 0 and var_posQuantità > 0 --CHARINDEX restituisce 0 se l'espressione non è trovata
			BEGIN
				SET var_specie = LTRIM(RTRIM(LEFT(var_listaSpecie, var_posSpecie - 1)))
				SET var_quantità = LTRIM(RTRIM(LEFT(var_quantità, var_posQuantità - 1))) AS INT
				IF var_specie <> '' and var_quantità <> ''
				BEGIN
					insert into `Contiene` (`numeroPacco`,`ordine`,`specie`,`quantità`)
					values (var_numeroPacco, var_ordine, var_specie, var_quantità);
				END
				SET var_listaSpecie = RIGHT(var_listaSpecie, LEN(var_listaSpecie) - var_posSpecie)
				SET var_listaQuantità = RIGHT(var_listaQuantità, LEN(var_listaQuantità) - var_posQuantità)
				SET var_posSpecie = CHARINDEX(',', var_listaSpecie, 1)
				SET var_posQuantità = CHARINDEX(',', var_listaQuantità, 1)
			END
			IF var_posQuantità <> 0 or var_posSpecie <> 0
			BEGIN
				signal sqlstate 45001 setmessagetext='Errore negli indici var_posSpecie o var_posQuantità'
			END
		END
		
		
	commit;
END