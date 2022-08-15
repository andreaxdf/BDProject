-- Selezione dei contatti di un dato contattabile.

CREATE PROCEDURE `seleziona_contatti` (
	in var_contattabile INT)
BEGIN

	declare exit handler for sqlexception
    begin
        rollback;
        resignal;
    end;

	set transaction isolation level read committed;
	set transaction read only;
    start transaction;
		select `tipo`, `valore`
		from `Contatto`
		where `contattabile`=var_contattabile;
	commit;
END