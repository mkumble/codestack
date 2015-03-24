-- MySQL dump 10.13  Distrib 5.5.9, for Win32 (x86)
--
-- Host: localhost    Database: krishisahayak
-- ------------------------------------------------------
-- Server version	5.1.56-community

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Current Database: `krishisahayak`
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `krishisahayak` /*!40100 DEFAULT CHARACTER SET latin1 */;

USE `krishisahayak`;

--
-- Table structure for table `accessoryid`
--

DROP TABLE IF EXISTS `accessoryid`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `accessoryid` (
  `powerid` varchar(5) DEFAULT NULL,
  `acc1` varchar(10) DEFAULT NULL,
  `acc2` varchar(10) DEFAULT NULL,
  `acc3` varchar(10) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `accessoryid`
--

LOCK TABLES `accessoryid` WRITE;
/*!40000 ALTER TABLE `accessoryid` DISABLE KEYS */;
INSERT INTO `accessoryid` VALUES ('300','a01trol','b03harr','c01grad'),('400','a02trol','b02harr','c01grad'),('500','a03trol','b02harr','c02grad'),('35','a01trol','b02harr','c01grad');
/*!40000 ALTER TABLE `accessoryid` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `accessoryname`
--

DROP TABLE IF EXISTS `accessoryname`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `accessoryname` (
  `name` varchar(15) DEFAULT NULL,
  `id` varchar(10) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `accessoryname`
--

LOCK TABLES `accessoryname` WRITE;
/*!40000 ALTER TABLE `accessoryname` DISABLE KEYS */;
INSERT INTO `accessoryname` VALUES ('trolley','a01'),('trolley','a02'),('trolley','a03'),('harvester','b01'),('harvester','b02'),('harvester','b03'),('grader','c01'),('grader','c02');
/*!40000 ALTER TABLE `accessoryname` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `crop`
--

DROP TABLE IF EXISTS `crop`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `crop` (
  `no` varchar(10) DEFAULT NULL,
  `name` varchar(10) DEFAULT NULL,
  `type` varchar(10) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `crop`
--

LOCK TABLES `crop` WRITE;
/*!40000 ALTER TABLE `crop` DISABLE KEYS */;
INSERT INTO `crop` VALUES ('1','pineapple','fruits'),('1','apple','fruits'),('2','corn','main'),('3','rose','flower'),('2','wheat','main'),('2','maize','main'),('1','drydates','fruits'),('2','paddy','main'),('4','cotton','comercial'),('4','jute','comercial'),('5','cotton','commercial'),('5','jute','commercial'),('6','ragi','main'),('6','bairley','main'),('7','sugarcane','commercial');
/*!40000 ALTER TABLE `crop` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `cropprice`
--

DROP TABLE IF EXISTS `cropprice`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `cropprice` (
  `name` varchar(10) DEFAULT NULL,
  `price` varchar(10) DEFAULT NULL,
  `trend` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `cropprice`
--

LOCK TABLES `cropprice` WRITE;
/*!40000 ALTER TABLE `cropprice` DISABLE KEYS */;
INSERT INTO `cropprice` VALUES ('Tomato','100',1),('Paddy','500',0),('onion','50',0),('Wheat','300',0),('SugarCane','700',1),('Rose','300',1);
/*!40000 ALTER TABLE `cropprice` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `enviornment`
--

DROP TABLE IF EXISTS `enviornment`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `enviornment` (
  `climate` varchar(10) DEFAULT NULL,
  `irrigation` varchar(10) DEFAULT NULL,
  `soil` varchar(10) DEFAULT NULL,
  `no` varchar(10) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `enviornment`
--

LOCK TABLES `enviornment` WRITE;
/*!40000 ALTER TABLE `enviornment` DISABLE KEYS */;
INSERT INTO `enviornment` VALUES ('humid','medium','alluvial','2'),('humid','medium','black','1'),('humid','medium','red','1'),('sunny','low','black','1'),('sunny','medium','alluvial','7'),('wet','high','red','2'),('humid','low','red','3'),('wet','medium','black','2'),('humid','low','black','4'),('humid','low','alluvial','4'),('sunny','low','alluvial','7'),('sunny','medium','black','6'),('wet','medium','red','1'),('wet','high','black','2');
/*!40000 ALTER TABLE `enviornment` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `farmer`
--

DROP TABLE IF EXISTS `farmer`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `farmer` (
  `farmerId` varchar(10) DEFAULT NULL,
  `place` varchar(10) DEFAULT NULL,
  `mobile` varchar(10) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `farmer`
--

LOCK TABLES `farmer` WRITE;
/*!40000 ALTER TABLE `farmer` DISABLE KEYS */;
/*!40000 ALTER TABLE `farmer` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `maintainence`
--

DROP TABLE IF EXISTS `maintainence`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `maintainence` (
  `model` varchar(10) DEFAULT NULL,
  `purchaseDate` date DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `maintainence`
--

LOCK TABLES `maintainence` WRITE;
/*!40000 ALTER TABLE `maintainence` DISABLE KEYS */;
INSERT INTO `maintainence` VALUES ('ld100','2010-03-10'),('md200','2010-03-10'),('mdh300','2010-03-10'),('h300','2010-03-10'),('vh400','2010-03-10'),('super1000','2010-03-10');
/*!40000 ALTER TABLE `maintainence` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `powerid`
--

DROP TABLE IF EXISTS `powerid`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `powerid` (
  `capacity` varchar(5) DEFAULT NULL,
  `powerid` varchar(5) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `powerid`
--

LOCK TABLES `powerid` WRITE;
/*!40000 ALTER TABLE `powerid` DISABLE KEYS */;
INSERT INTO `powerid` VALUES ('30','300'),('40','400'),('50','500'),('60','600'),('70','700'),('20','35');
/*!40000 ALTER TABLE `powerid` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2011-05-02 21:10:38
