-- MySQL dump 10.13  Distrib 8.0.46, for Linux (x86_64)
--
-- Host: localhost    Database: test_innodb
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
-- Table structure for table `t_archive`
--

DROP TABLE IF EXISTS `t_archive`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `t_archive` (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` varchar(20) COLLATE utf8mb4_general_ci DEFAULT NULL,
  UNIQUE KEY `id` (`id`)
) ENGINE=ARCHIVE DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_archive`
--

LOCK TABLES `t_archive` WRITE;
/*!40000 ALTER TABLE `t_archive` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_archive` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_blackhole`
--

DROP TABLE IF EXISTS `t_blackhole`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `t_blackhole` (
  `id` int DEFAULT NULL,
  `conten` char(10) COLLATE utf8mb4_general_ci DEFAULT NULL
) ENGINE=BLACKHOLE DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_blackhole`
--

LOCK TABLES `t_blackhole` WRITE;
/*!40000 ALTER TABLE `t_blackhole` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_blackhole` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_csv`
--

DROP TABLE IF EXISTS `t_csv`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `t_csv` (
  `id` int NOT NULL,
  `comtent` char(100) COLLATE utf8mb4_general_ci NOT NULL
) ENGINE=CSV DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_csv`
--

LOCK TABLES `t_csv` WRITE;
/*!40000 ALTER TABLE `t_csv` DISABLE KEYS */;
INSERT INTO `t_csv` VALUES (1,'record one'),(2,'record two');
/*!40000 ALTER TABLE `t_csv` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_innodb`
--

DROP TABLE IF EXISTS `t_innodb`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `t_innodb` (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` varchar(20) COLLATE utf8mb4_general_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_innodb`
--

LOCK TABLES `t_innodb` WRITE;
/*!40000 ALTER TABLE `t_innodb` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_innodb` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_m1`
--

DROP TABLE IF EXISTS `t_m1`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `t_m1` (
  `id` int NOT NULL AUTO_INCREMENT,
  `content` char(20) COLLATE utf8mb4_general_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_m1`
--

LOCK TABLES `t_m1` WRITE;
/*!40000 ALTER TABLE `t_m1` DISABLE KEYS */;
INSERT INTO `t_m1` VALUES (1,'Testing1'),(2,'table1'),(3,'test_m1');
/*!40000 ALTER TABLE `t_m1` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_m2`
--

DROP TABLE IF EXISTS `t_m2`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `t_m2` (
  `id` int NOT NULL AUTO_INCREMENT,
  `content` char(20) COLLATE utf8mb4_general_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_m2`
--

LOCK TABLES `t_m2` WRITE;
/*!40000 ALTER TABLE `t_m2` DISABLE KEYS */;
INSERT INTO `t_m2` VALUES (1,'Testing2'),(2,'table2'),(3,'test_m2');
/*!40000 ALTER TABLE `t_m2` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_memory1`
--

DROP TABLE IF EXISTS `t_memory1`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `t_memory1` (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` varchar(20) COLLATE utf8mb4_general_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MEMORY DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_memory1`
--

LOCK TABLES `t_memory1` WRITE;
/*!40000 ALTER TABLE `t_memory1` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_memory1` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_merge`
--

DROP TABLE IF EXISTS `t_merge`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `t_merge` (
  `id` int NOT NULL AUTO_INCREMENT,
  `content` char(20) COLLATE utf8mb4_general_ci DEFAULT NULL,
  KEY `id` (`id`)
) ENGINE=MRG_MyISAM DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci INSERT_METHOD=LAST UNION=(`t_m1`,`t_m2`);
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `t_myisam`
--

DROP TABLE IF EXISTS `t_myisam`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `t_myisam` (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` varchar(20) COLLATE utf8mb4_general_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_myisam`
--

LOCK TABLES `t_myisam` WRITE;
/*!40000 ALTER TABLE `t_myisam` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_myisam` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2026-05-23 21:58:09
