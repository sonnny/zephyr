//
// part of pubspec.yaml
//
// dependencies:
//   flutter:
//     sdk: flutter
//   arrow_pad:
//     git:
//       url: https://github.com/sonnny/arrow_pad.git
//   awesome_number_picker:

import 'package:awesome_number_picker/awesome_number_picker.dart';
import 'package:flutter/material.dart';
import 'package:arrow_pad/arrow_pad.dart';
import 'dart:io';
import './scroll_behaviour.dart';

void main() { runApp(MyApp()); }

class MyApp extends StatelessWidget {
  MyApp({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      scrollBehavior: CustomScrollBehavior(), //For web Scrolling
      home: HomePage(),);}}

class HomePage extends StatefulWidget {
  const HomePage({Key? key}) : super(key: key);

  @override
  State<HomePage> createState() => _HomePageState();}

class _HomePageState extends State<HomePage>  {

  int integerValue1 = 0;
  int integerValue2 = 1; 
  late RawDatagramSocket socket;

  @override void initState(){
    super.initState();
    _initSocket();}
    
  Future<void> _initSocket() async {
    try {
       socket = await RawDatagramSocket.bind(InternetAddress.anyIPv4, 0);
       
    } catch (e) {
      print('error');}}    
  
Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text("demo zephyr pico2w")),
      body: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        crossAxisAlignment: CrossAxisAlignment.center,
        children: [
          Text('enter pico2w address from oled screen'),
          Expanded(child: Row(
          mainAxisAlignment: MainAxisAlignment.spaceAround,
            crossAxisAlignment: CrossAxisAlignment.center,
          children:[
          
          Text('ip address: 192.168.'+integerValue1.toString()+'.'+integerValue2.toString(), style: TextStyle(fontSize:22, fontWeight: FontWeight.bold,color: Colors.teal)),
         SizedBox(
            height: 150, width: 50,
            
            child: IntegerNumberPicker(
              initialValue: 1,
              minValue: 1,
              maxValue: 10,
              onChanged: (i) => setState(() {
                integerValue1 = i;
              }),
            ),
          ),
          SizedBox(height:150, width: 50,
            child: IntegerNumberPicker(
              initialValue: 5,
              minValue: 1,
              maxValue: 50,
              onChanged: (i) => setState(() {
                integerValue2 = i;
              }),),),])),
          
          ArrowPad(onDirectionTap:(direction) async {
            socket.send(direction.codeUnits, InternetAddress('192.168.'+integerValue1.toString()+'.'+integerValue2.toString()), 8001);
          }),
          
          SizedBox(height:30),
          
          OutlinedButton(onPressed:() => socket.send('stop'.codeUnits,InternetAddress('192.168.'+integerValue1.toString()+'.'+integerValue2.toString()), 8001), child:Text('stop')),
          
          SizedBox(height:20)
          
        ],),);}}
