#MySQL

1. Create DB from file
  ```
  mysql -u root -p < start_up_script.sql
  ```

2. MySQL Keywords
 * SELECT DISTINCT column1, ...
   * DISTINCT treats two NULL-s as equal, thus duplicate
   * DISTINCT can be followed by multiply columns
 * FROM, WHERE
 * ORDER BY columns [ASC|DESC], ...
 * AS 
   * create an alias
 * FORMAT(arithmetic, precision)
   * turn into string
 * ROUND
 

3. SELECT 
```
SELECT column1, column2, ...
FROM
WHERE 
ORDER BY column1 [ASC|DESC]
```
