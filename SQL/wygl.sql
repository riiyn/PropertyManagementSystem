/*
Navicat MySQL Data Transfer

Source Server         : qt
Source Server Version : 50555
Source Host           : localhost:3306
Source Database       : wygl

Target Server Type    : MYSQL
Target Server Version : 50555
File Encoding         : 65001

Date: 2019-03-14 19:59:53
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for admin
-- ----------------------------
DROP TABLE IF EXISTS `admin`;
CREATE TABLE `admin` (
  `aid` int(20) NOT NULL AUTO_INCREMENT,
  `aname` varchar(20) NOT NULL,
  `apassword` varchar(20) NOT NULL,
  `arealname` varchar(20) NOT NULL,
  `tel` varchar(20) NOT NULL,
  `status` varchar(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (`aid`)
) ENGINE=InnoDB AUTO_INCREMENT=21 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for car
-- ----------------------------
DROP TABLE IF EXISTS `car`;
CREATE TABLE `car` (
  `cid` int(20) NOT NULL AUTO_INCREMENT,
  `realname` varchar(20) NOT NULL,
  `cname` varchar(20) NOT NULL,
  `cnum` varchar(20) NOT NULL,
  `p_num` int(50) NOT NULL,
  PRIMARY KEY (`cid`),
  KEY `cname` (`cname`,`cnum`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for charge
-- ----------------------------
DROP TABLE IF EXISTS `charge`;
CREATE TABLE `charge` (
  `cha_id` int(20) NOT NULL AUTO_INCREMENT,
  `cha_name` varchar(50) NOT NULL,
  `cha_standard` varchar(20) NOT NULL,
  PRIMARY KEY (`cha_id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for complaints
-- ----------------------------
DROP TABLE IF EXISTS `complaints`;
CREATE TABLE `complaints` (
  `comp_id` int(20) NOT NULL AUTO_INCREMENT,
  `uid` int(20) NOT NULL,
  `realname` varchar(20) NOT NULL,
  `comp_text` text NOT NULL,
  `comp_date` varchar(50) NOT NULL,
  `comp_state` int(11) NOT NULL COMMENT '投诉状态，1已解决 0未解决 ',
  PRIMARY KEY (`comp_id`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for house
-- ----------------------------
DROP TABLE IF EXISTS `house`;
CREATE TABLE `house` (
  `hid` int(20) NOT NULL AUTO_INCREMENT,
  `hname` varchar(20) NOT NULL,
  `buildStarttime` datetime NOT NULL,
  `buildEndtime` datetime NOT NULL,
  `harea` float NOT NULL,
  PRIMARY KEY (`hid`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for log
-- ----------------------------
DROP TABLE IF EXISTS `log`;
CREATE TABLE `log` (
  `logid` int(20) NOT NULL AUTO_INCREMENT,
  `log_name` varchar(20) NOT NULL,
  `log_counts` int(20) NOT NULL,
  `last_date` varchar(50) NOT NULL,
  `last_ip` varchar(80) NOT NULL,
  `log_play` varchar(20) NOT NULL COMMENT '登陆者角色，管理员/用户',
  PRIMARY KEY (`logid`),
  KEY `log_name` (`log_name`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for matter
-- ----------------------------
DROP TABLE IF EXISTS `matter`;
CREATE TABLE `matter` (
  `mid` int(20) NOT NULL AUTO_INCREMENT,
  `mname` varchar(50) NOT NULL,
  `mnum` int(11) NOT NULL,
  `mprice` float NOT NULL,
  `ins_time` datetime NOT NULL,
  `outs_time` datetime DEFAULT NULL,
  `mstate` varchar(50) NOT NULL COMMENT '物资状态 1使用中，2在库，3维修，0废弃',
  PRIMARY KEY (`mid`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for pay
-- ----------------------------
DROP TABLE IF EXISTS `pay`;
CREATE TABLE `pay` (
  `pid` int(20) NOT NULL AUTO_INCREMENT,
  `cha_name` varchar(50) NOT NULL,
  `cha_standard` float NOT NULL,
  `preal` float NOT NULL,
  `pbalance` float NOT NULL,
  `cha_time` datetime NOT NULL,
  `uid` int(20) NOT NULL,
  `pstate` varchar(2) NOT NULL COMMENT '缴费状态',
  `pmonth` varchar(50) NOT NULL COMMENT '缴费月份',
  PRIMARY KEY (`pid`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for room
-- ----------------------------
DROP TABLE IF EXISTS `room`;
CREATE TABLE `room` (
  `rid` int(20) NOT NULL AUTO_INCREMENT,
  `rnum` varchar(20) NOT NULL,
  `hid` int(20) NOT NULL,
  `uid` int(20) DEFAULT NULL,
  `enterTime` datetime DEFAULT NULL,
  `rtype` varchar(20) NOT NULL,
  `rarea` float NOT NULL,
  PRIMARY KEY (`rid`),
  KEY `r` (`uid`),
  KEY `room` (`hid`),
  CONSTRAINT `r` FOREIGN KEY (`uid`) REFERENCES `user` (`uid`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `room` FOREIGN KEY (`hid`) REFERENCES `house` (`hid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=37 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for user
-- ----------------------------
DROP TABLE IF EXISTS `user`;
CREATE TABLE `user` (
  `uid` int(20) NOT NULL AUTO_INCREMENT,
  `username` varchar(20) NOT NULL,
  `password` varchar(20) NOT NULL,
  `realname` varchar(20) NOT NULL,
  `sex` varchar(2) NOT NULL,
  `age` int(20) NOT NULL,
  `address` varchar(50) NOT NULL,
  `house_id` int(50) NOT NULL,
  `unit_id` int(50) NOT NULL,
  `room_id` int(50) NOT NULL,
  `userIDcard` varchar(50) NOT NULL,
  `tel` varchar(20) NOT NULL,
  `workplace` varchar(50) NOT NULL COMMENT '业主工作地',
  `IC_Num` varchar(50) DEFAULT NULL,
  `statu` varchar(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (`uid`),
  KEY `realname` (`realname`)
) ENGINE=InnoDB AUTO_INCREMENT=18 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for userlogs
-- ----------------------------
DROP TABLE IF EXISTS `userlogs`;
CREATE TABLE `userlogs` (
  `id` int(50) NOT NULL AUTO_INCREMENT,
  `uid` int(50) NOT NULL,
  `name` varchar(50) NOT NULL,
  `logTime` datetime NOT NULL,
  `logIP` varchar(50) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `userlogs` (`uid`)
) ENGINE=InnoDB AUTO_INCREMENT=60 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for user_family
-- ----------------------------
DROP TABLE IF EXISTS `user_family`;
CREATE TABLE `user_family` (
  `ufid` int(20) NOT NULL AUTO_INCREMENT,
  `ufname` varchar(20) NOT NULL,
  `ufsex` varchar(2) NOT NULL,
  `uftel` varchar(20) NOT NULL,
  `uf_RelatW _user` varchar(20) NOT NULL COMMENT '与业主关系',
  `uid` int(20) NOT NULL,
  PRIMARY KEY (`ufid`),
  KEY `user_family` (`uid`),
  CONSTRAINT `user_family` FOREIGN KEY (`uid`) REFERENCES `user` (`uid`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for xiaoqu
-- ----------------------------
DROP TABLE IF EXISTS `xiaoqu`;
CREATE TABLE `xiaoqu` (
  `xqid` int(10) NOT NULL AUTO_INCREMENT,
  `xqname` varchar(60) NOT NULL COMMENT '小区名称',
  `name` varchar(20) NOT NULL COMMENT '小区负责人姓名',
  `sex` varchar(2) NOT NULL,
  `tel` varchar(20) NOT NULL,
  `username` varchar(20) NOT NULL,
  `pwd` varchar(20) NOT NULL,
  `acl` varchar(20) NOT NULL COMMENT '登陆角色',
  `xqtext` text NOT NULL COMMENT '小区简介',
  `kftel` varchar(20) NOT NULL COMMENT '客服电话',
  `op_time` varchar(50) NOT NULL COMMENT '操作时间',
  PRIMARY KEY (`xqid`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
