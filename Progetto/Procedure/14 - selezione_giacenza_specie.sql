-- Selezione delle specie ordinate per giacenza crescente.

CREATE PROCEDURE `selezione_giacenza_specie` ()
BEGIN

	declare exit handler for sqlexception
    begin
        rollback; 
        resignal;
    end;

	set transaction isolation level serializable;
	set transaction read only;
    start transaction;
		select `codiceSpecie`, `nomeLatino`, `nomeComune`, `giacenza`
		from `Specie`
		order by giacenza desc;
	commit;
END