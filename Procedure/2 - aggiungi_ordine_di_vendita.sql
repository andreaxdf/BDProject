-- Inserimento di un nuovo ordine di vendita 
-- (diminuendo la giacenza delle specie coinvolte).

CREATE PROCEDURE `aggiungi_ordine_di_vendita` ( 
	in var_indirizzoSpedizione VARCHAR(100),
	in var_cliente INT,
	in var_contatto VARCHAR(45),
	in var_lista_specie VARCHAR(2000),
	in var_lista_quantità VARCHAR(1000))
BEGIN
	declare var_codiceOrdine int;
	declare var_posSpecie bigint;
    declare var_posQuantità bigint;
    declare var_specie varchar(10);
    declare var_quantità int;
    declare var_temp varchar(10);
    declare var_data_prezzo date;

	declare exit handler for sqlexception
    begin
        rollback; 
        resignal;
    end;

	set transaction isolation level serializable;
    start transaction;
		insert into `OrdineDiVendita` (`indirizzoSpedizione`, `data`, `cliente`, `contatto`)
		values (var_indirizzoSpedizione, curdate(), var_cliente, var_contatto);
		set var_codiceOrdine = last_insert_id();
        
		SET var_lista_specie = LTRIM(RTRIM(var_lista_specie))+ ',';
		SET var_lista_quantità = LTRIM(RTRIM(var_lista_quantità))+ ',';
		SET var_posSpecie = position(',' IN var_lista_specie);
		SET var_posQuantità = position(',' IN var_posQuantità);
 
		IF REPLACE(var_lista_specie, ',', '') <> '' THEN -- togliere if? cosa fa?
			WHILE var_posSpecie > 0 and var_posQuantità > 0 DO 
				SET var_specie = LTRIM(RTRIM(LEFT(var_lista_specie, var_posSpecie - 1)));
				SET var_temp = LTRIM(RTRIM(LEFT(var_quantità, var_posQuantità - 1)));
                SET var_quantità = var_temp * 1;  -- cast varchar -> int
				select `dataPrezzoAttuale`
                from `Specie`
                where `codiceSpecie` = var_specie into var_data_prezzo;
				IF var_specie <> '' and var_temp <> '' THEN
					insert into `Richiede` (`specie`,`ordine`,`dataPrezzo`,`quantità`)
					values (var_specie, var_codiceOrdine, var_data_prezzo, var_quantità);
				END IF;
				SET var_lista_specie = RIGHT(var_lista_specie, length(var_lista_specie) - var_posSpecie);
				SET var_lista_quantità = RIGHT(var_lista_quantità, length(var_lista_quantità) - var_posQuantità);
				SET var_posSpecie = position(',' IN var_lista_specie);
				SET var_posQuantità = position(',' IN var_lista_quantità);
			END WHILE;
			IF var_posQuantità <> 0 or var_posSpecie <> 0 THEN
				 signal sqlstate '45001' set message_text='Errore nelle liste date in input. Inserire per ogni specie anche una quantità';
			END IF;
		END IF;
	commit;
END