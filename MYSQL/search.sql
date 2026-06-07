-- MySQL dump 10.13  Distrib 8.0.46, for Linux (x86_64)
--
-- Host: localhost    Database: scott
-- ------------------------------------------------------
-- Server version	8.0.46

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `dept`
--

DROP TABLE IF EXISTS `dept`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `dept` (
  `deptno` int NOT NULL COMMENT '部门编号，主键',
  `dname` varchar(14) DEFAULT NULL COMMENT '部门名称',
  `loc` varchar(13) DEFAULT NULL COMMENT '部门所在地点',
  PRIMARY KEY (`deptno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='部门信息表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `dept`
--

LOCK TABLES `dept` WRITE;
/*!40000 ALTER TABLE `dept` DISABLE KEYS */;
INSERT INTO `dept` VALUES (10,'accounting','new york'),(20,'research','dallas'),(30,'sales','chicago'),(40,'operations','boston');
/*!40000 ALTER TABLE `dept` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `emp`
--

DROP TABLE IF EXISTS `emp`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `emp` (
  `empno` int NOT NULL COMMENT '员工编号，主键',
  `ename` varchar(10) DEFAULT NULL COMMENT '员工姓名',
  `job` varchar(9) DEFAULT NULL COMMENT '职位',
  `mgr` int DEFAULT NULL COMMENT '上级领导编号，引用empno',
  `hiredate` date DEFAULT NULL COMMENT '入职日期',
  `sal` decimal(7,2) DEFAULT NULL COMMENT '工资',
  `comm` decimal(7,2) DEFAULT NULL COMMENT '奖金/佣金',
  `deptno` int DEFAULT NULL COMMENT '所属部门编号，外键关联dept表',
  PRIMARY KEY (`empno`),
  KEY `deptno` (`deptno`),
  KEY `mgr` (`mgr`),
  CONSTRAINT `emp_ibfk_1` FOREIGN KEY (`deptno`) REFERENCES `dept` (`deptno`),
  CONSTRAINT `emp_ibfk_2` FOREIGN KEY (`mgr`) REFERENCES `emp` (`empno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='员工信息表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `emp`
--

LOCK TABLES `emp` WRITE;
/*!40000 ALTER TABLE `emp` DISABLE KEYS */;
INSERT INTO `emp` VALUES (7369,'smith','clerk',7902,'1980-12-17',800.00,NULL,20),(7499,'allen','salesman',7698,'1981-02-20',1600.00,300.00,30),(7521,'ward','salesman',7698,'1981-02-22',1250.00,500.00,30),(7566,'jones','manager',7839,'1981-04-02',2975.00,NULL,20),(7654,'martin','salesman',7698,'1981-09-28',1250.00,1400.00,30),(7698,'blake','manager',7839,'1981-05-01',2850.00,NULL,30),(7782,'clark','manager',7839,'1981-06-09',2450.00,NULL,10),(7788,'scott','analyst',7566,'1982-12-09',3000.00,NULL,20),(7839,'king','president',NULL,'1981-11-17',5000.00,NULL,10),(7844,'turner','salesman',7698,'1981-09-08',1500.00,0.00,30),(7876,'adams','clerk',7788,'1983-01-12',1100.00,NULL,20),(7900,'james','clerk',7698,'1981-12-03',950.00,NULL,30),(7902,'ford','analyst',7566,'1981-12-03',3000.00,NULL,20),(7934,'miller','clerk',7782,'1982-01-23',1300.00,NULL,10);
/*!40000 ALTER TABLE `emp` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `salgrade`
--

DROP TABLE IF EXISTS `salgrade`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `salgrade` (
  `grade` int NOT NULL COMMENT '工资等级',
  `losal` int DEFAULT NULL COMMENT '该等级的最低工资',
  `hisal` int DEFAULT NULL COMMENT '该等级的最高工资',
  PRIMARY KEY (`grade`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='工资等级表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `salgrade`
--

LOCK TABLES `salgrade` WRITE;
/*!40000 ALTER TABLE `salgrade` DISABLE KEYS */;
INSERT INTO `salgrade` VALUES (1,700,1200),(2,1201,1400),(3,1401,2000),(4,2001,3000),(5,3001,9999);
/*!40000 ALTER TABLE `salgrade` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2026-05-15 21:25:29
