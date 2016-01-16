#MySQL

1. Create DB from file
  ```
  mysql -u root -p < start_up_script.sql
  ```

2. MySQL Keywords
 * SELECT DISTINCT column1, ...
   * DISTINCT treats two NULL-s as equal, thus duplicate
   * DISTINCT can be followed by multiply columns
 * NULL
   * Cannot compare using =(equal) sign
   * WHERE nullColumn IS NULL
   * WHERE nullColumn IS NOT NULL
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
 

3. SELECT 
```
SELECT column1, column2, ...
FROM
WHERE 
ORDER BY column1 [ASC|DESC]
```
