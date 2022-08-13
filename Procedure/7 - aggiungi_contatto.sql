-- Inserimento di un nuovo contatto per un contattabile.

CREATE PROCEDURE `aggiungi_contatto` (
	in var_contattabile INT,
	in var_valore VARCHAR(45),
	in var_tipo VARCHAR(1))
BEGIN

	declare exit handler for sqlexception
    begin
        rollback; 
        resignal;
    end;

	set transaction isolation level serializable;
    start transaction;
		insert into `Contatto` (`contattabile`, `valore`, `tipo`)
		values (var_contattabile, var_valore, var_tipo);
	commit;
END