--Op.5: Inserimento nuovo pacco.

CREATE PROCEDURE `aggiungi_pacco` (
	in var_ordine INT, 
	in var_numeroPacco INT,
	in var_listaSpecie VARCHAR(2000),
	in var_listaQuantità VARCHAR(1000))
BEGIN
	declare var_posSpecie bigint;
    declare var_posQuantità bigint;
    declare var_specie varchar(10);
    declare var_quantità int;
    declare var_temp varchar(10);

	declare exit handler for sqlexception
    begin
        rollback;
        resignal;
    end;

	set transaction isolation level serializable;
    start transaction;
		insert into `Pacco` (`numeroPacco`, `ordine`)
		values (var_numeroPacco, var_ordine);
		
		
 
		SET var_listaSpecie = LTRIM(RTRIM(var_listaSpecie))+ ',';
		SET var_listaQuantità = LTRIM(RTRIM(var_listaQuantità))+ ',';
		SET var_posSpecie = position(',' IN var_listaSpecie);
		SET var_posQuantità = position(',' IN var_posQuantità);
 
		IF REPLACE(var_listaSpecie, ',', '') <> '' THEN
			WHILE var_posSpecie > 0 and var_posQuantità > 0 DO 
				SET var_specie = LTRIM(RTRIM(LEFT(var_listaSpecie, var_posSpecie - 1)));
				SET var_temp = LTRIM(RTRIM(LEFT(var_quantità, var_posQuantità - 1)));
                SET var_quantità = var_temp * 1;  -- cast varchar -> int
				IF var_specie <> '' and var_quantità <> '' THEN
					insert into `Contiene` (`numeroPacco`,`ordine`,`specie`,`quantità`)
					values (var_numeroPacco, var_ordine, var_specie, var_quantità);
				END IF;
				SET var_listaSpecie = RIGHT(var_listaSpecie, length(var_listaSpecie) - var_posSpecie);
				SET var_listaQuantità = RIGHT(var_listaQuantità, length(var_listaQuantità) - var_posQuantità);
				SET var_posSpecie = position(',' IN var_listaSpecie);
				SET var_posQuantità = position(',' IN var_listaQuantità);
			END WHILE;
			-- IF var_posQuantità <> 0 or var_posSpecie <> 0 THEN
				-- signal sqlstate=45001 setmessagetext='Errore negli indici var_posSpecie o var_posQuantità';
			-- END IF;
		END IF;
		
		
	commit;
END