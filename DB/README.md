#MySQL

1. Create DB from file
  ```
  mysql -u root -p < start_up_script.sql
  ```

2. Database Administration
 * SHOW DATABASES
 * USE t1
 * SHOW COLUMNS FROM t1
 
3. CREATE
   ``` 
   DROP TABLE IF EXISTS t1
   
   CREATE TABLE t1 (column-list) engine = type
   ```
   Since MySQL 5.5, the default engine is InnoDB. Before, it is MyISAM.
   ```
   column-list: column
             | column , column-list
   
   column: ID TYPE [[NOT] NULL] [DEFAULT val] [COMMENT STRING] [AUTOINCREMENT] 
         | PRIMARY KEY (IDs)
         | KEY ID (IDs)
   ```

4. SELECT
 * SELECT DISTINCT column1, ...
   * DISTINCT treats two NULL-s as equal, thus duplicate
   * DISTINCT can be followed by multiply columns
 * NULL
   * Cannot compare using =(equal) sign
   * WHERE nullColumn IS NULL
   * WHERE nullColumn IS NOT NULL
   * Some subtlity with NULL: 1. All columns can be made NULL. 2. Whenever there's a NULL on an arbitrary column c, WHERE c < v1 or c >= v2 cannot fetch the entry with NULL. 3. NULL is a distinct value. 4. COUNT doesn't include NULL
 * GROUP BY column1
   * This is very similar to DISTINCT column. The major difference is that GROUP BY will sort the entries
 * HAVING
   * HAVING expr 
   * Used with GROUP BY
 * SUM, COUNT, AVG, ...
   * Aggregate functions. Work the best with DISTINCT and GROUP BY.
 * LIMIT offset, count | LIMIT count
   * Select first count entries. If offset is specified, skip offset entries
   * offset and count must be non-negative integer
   * Very handy to select top nth entries and the nth entry
 * IN
   * (expr|column) in (value1, value2, ...)
   * This is a syntax suger. You can write ``` WHERE c = v1 or c = v2 or ... ```
   * ... WHERE c1 IN (SUBQUERY) 
   * Whenever one does a SUBQUERY, the query can usually be written by JOIN. Typically JOIN is more efficient, but it is not as readable as using a SUBQUERY.
 * NOT
   * Used to negate a predicate 
 * BETWEEN a AND b
   * Syntax suger. ``` WHERE c >= a AND c <= b ``` 
 * FROM, WHERE
 * LIKE pattern
    * '%' match none or any string
    * '_' match any single character
    * '\' comes to escape the wildcard character
    * DB has to scan through all entries. Notice the efficiency downgrade
 * ESCAPE
 * CAST
 * ORDER BY columns [ASC|DESC], ...
 * AS 
   * create an alias
   * column alias can be used in ORDER BY, GROUP BY, and HAVING
 * FORMAT(arithmetic, precision)
   * turn into string
 * ROUND
 * CONCAT_WS (delim, str1, str2, ...)
 * INNER JOIN 
   * The same as JOIN
   * JOIN T1, T2 ON ...
   * JOIN two tables where c1 and c2 are equal 
 * NATURAL JOIN T1, T2 [USING(attr)]
   * Equal to JOIN T1, T2 ON ...
   * USING clause can be omitted but is recommended for better style
 * [LEFT|RIGHT|FULL] OUTER JOIN  
 

5. INSERT
 * INSERT INTO t1 VALUES (t1, t2, ...)
 * INSERT INTO t1 select-clause
 * The underlying schema must be the same

6. DELETE
 * DELETE FROM t1 WHERE ...

7. UPDATE
 * UPDATE TABLE SET (attr = expr)+ WHERE ...
 
8. Constraints
 * Constrain allowable database states (static)
 * Declaration with original schema or later
 * NOT NULL
 * PRIMARY KEY 
   * Must be unique and only one column can be primary key; Can define composite key; duplicate NULL is allowed to some systems)
 * UNIQUE (duplicate NULL is allowed)
 * CHECK (condition)
   * Many systems (maybe no systems) support subsqueries in CHECK
 * ASSERTION KEY (Not supported)
 * Referencial Integrity
   * If referencing table generates error, then error; if referenced table generates error, then RESTRICT (by default) |SET NULL|CASCADE 

9. Trigger
 * Monitor database changes (dynamic)
 * Move logic from application to database
 * 
 ``` 
  CREATE Trigger name
  BEFORE|AFTER|INSTEAD OF INSERT ON table|DELETE ON table|UPDATE ON table
  [ reference variables ]
  [ for each row ]
  WHEN (condition)
  action
  ```
 
10. Transaction
 * ACID
 * Atomicity (a crash during a transaction won't leave any half-way-done statements); 
 * Consistency (constraint always holds); 
 * Isolation (quries may be interleaved but there will be limited equavalent sequence); 
 * READ UNCOMMITTED (allow dirty read)
 * READ COMMITED (may not allow dirty read, but cannot guarantee global serializability)
 * REPEATABLE READ (no dirty read; multiple read won't change value, again no guarantee)
 * SERIALIZABLE (strongest)
 * READ ONLY
 * Durability (a crash won't affect any commited transaction);
 * Rollback (undo partial effects of transaction)
 
11. Views
 * Physical (binary), Conceptual (relations), Logical (views)
 * Enhance encapsulation, modularity, query
 * Modify views through triggers (CREATE TRIGGER T INSTEAD OF ...)
 * Modify views which contain aggregation (Disallow modificiation)
 * System won't enforce the correctness of Triggers to do the modification
 ```
 CREATE VIEW Vname AS <query> 
 ```
 
