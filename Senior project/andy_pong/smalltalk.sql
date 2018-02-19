-- MySQL dump 10.13  Distrib 5.7.20, for Linux (x86_64)
--
-- Host: localhost    Database: SmallTalk
-- ------------------------------------------------------
-- Server version	5.7.20-0ubuntu0.16.04.1

--
-- Table structure for table `96milliondollarpizza_2PD`
--

CREATE TABLE `96milliondollarpizza_2PD` (
  `First` varchar(255) NOT NULL,
  `Last` varchar(255) NOT NULL,
  `email` varchar(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Table structure for table `Students`
--

CREATE TABLE `Students` (
  `email` varchar(255) NOT NULL,
  `Lastname` varchar(255) NOT NULL,
  `Firstname` varchar(255) NOT NULL,
  `PD` int(11) DEFAULT NULL,
  `Talks` varchar(255) DEFAULT NULL,
  `PW` varchar(255) NOT NULL,
  PRIMARY KEY (`email`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Table structure for table `Talk_Tracker`
--

CREATE TABLE `Talk_Tracker` (
  `First` varchar(255) NOT NULL,
  `Last` varchar(255) NOT NULL,
  `email` varchar(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

