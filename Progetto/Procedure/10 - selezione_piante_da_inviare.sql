-- Selezione delle piante che sono state richieste in un ordine, 
-- ma che non sono ancora state inserite in pacchi.

CREATE PROCEDURE `selezione_piante_da_inviare` (
	in var_codiceOrdine INT,
	out table_piante_da_inviare)
BEGIN
	declare exit handler for sqlexception
    begin
        rollback; 
        resignal;
    end;
	
	declare done int default false;
	declare var_numeroPacco INT;
	declare var_specie VARCHAR(10);
	declare var_quantità INT;
	declare var_quantita2 INT;
	
	-- possiamo togliere questo cursore e accedere direttamente a Contiene tramite il codice ordine
	declare cur_pacchi cursor for
		select `numeroPacco`
		from `Pacco`
		where `ordine` = var_codiceOrdine;
			
	declare continue handler for not found set done = true;
			
	DROP TABLE IF EXISTS `PianteRichieste`;
    CREATE TEMPORARY TABLE `PianteRichieste`(
		`specie` VARCHAR(10),
        `quantità` INT);
		
	DROP TABLE IF EXISTS `PianteInviate`;
    CREATE TEMPORARY TABLE `PianteInviate`(
		`specie` VARCHAR(10),
        `quantità` INT);

	set transaction isolation level serializable;
    start transaction;
		
		open cur_pacchi;
		
		read_loop: loop
			fetch cur_pacchi into var_numeroPacco;
			if done then
				leave read_loop;
			end if;
		   
			declare cur_piante cursor for 
				select `specie`, `quantità`
				from `Contiene`
				where `ordine`=var_codiceOrdine 
				and `numeroPacco`=var_numeroPacco;
			
			open cur_piante;
			
			read_loop2: loop
				set var_quantita2 = 0;
				fetch cur_piante into var_specie, var_quantità;
				if done then
					leave read_loop2;
				end if;
				----------- Da rivedere
				select `quantità`
				from `PianteInviate`
				where `specie`=var_specie into var_quantita2;
				if var_quantita2 <> 0
					update `PianteInviate` 
						set `quantità` = var_quantita2+var_quantità
						where `specie` = var_specie;
				else
					insert into `PianteInviate` (`specie`, `quantità`)
					values (var_specie, var_quantità);
				end if;
				-----------------------
			end loop;
			set done = false;

		end loop;
		
		
		insert into `PianteRichieste`
			select `specie`, `quantità`
			from `Richiede`
			where `ordine`=var_codiceOrdine;
		
		-- fare sottrazione delle quantità tra piante richieste e piante inviate
		
		select `specie`, `PianteRichieste`.`quantità` - `PianteInviate`.`quantità`
		from `PianteInviate` join `PianteRichieste` on `PianteInviate`.`specie` = `PianteRichieste`.`specie`; --cosa succede se PianteInviate.quantità == null?
		
	commit;
END