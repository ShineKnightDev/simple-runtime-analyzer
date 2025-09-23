/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "Simple Runtime Analyzer", "index.html", [
    [ "Overview", "index.html#autotoc_md1", [
      [ "Motivation", "index.html#autotoc_md3", null ],
      [ "Features", "index.html#autotoc_md5", null ],
      [ "Library Modules", "index.html#autotoc_md7", null ],
      [ "Output Formats", "index.html#autotoc_md9", null ],
      [ "Examples", "index.html#autotoc_md11", null ],
      [ "Getting Started", "index.html#autotoc_md13", [
        [ "Requirements", "index.html#autotoc_md14", null ],
        [ "Installation", "index.html#autotoc_md15", [
          [ "C++ Library Usage", "index.html#autotoc_md16", null ],
          [ "Plotting Script Usage", "index.html#autotoc_md17", null ]
        ] ]
      ] ],
      [ "Project Tree", "index.html#autotoc_md19", null ],
      [ "Building the Project", "index.html#autotoc_md21", null ],
      [ "Documentation", "index.html#autotoc_md23", null ],
      [ "Community & Contributing", "index.html#autotoc_md25", null ]
    ] ],
    [ "Usage Notes", "md_docs_2usage__notes.html", [
      [ "General Recommendations", "md_docs_2usage__notes.html#autotoc_md27", null ],
      [ "Tips for Runtime Analyzer", "md_docs_2usage__notes.html#autotoc_md29", null ],
      [ "Tips for Sample Utilities", "md_docs_2usage__notes.html#autotoc_md31", null ],
      [ "Tips for Runtime Reporter", "md_docs_2usage__notes.html#autotoc_md33", null ]
    ] ],
    [ "Runtime Analyzer Module", "md_docs_2runtime__analyzer.html", [
      [ "Key Features", "md_docs_2runtime__analyzer.html#autotoc_md35", null ],
      [ "Concepts and Type Requirements", "md_docs_2runtime__analyzer.html#autotoc_md37", null ],
      [ "Core Components", "md_docs_2runtime__analyzer.html#autotoc_md39", [
        [ "RuntimeProfile Structure", "md_docs_2runtime__analyzer.html#autotoc_md40", null ],
        [ "measure_duration Function", "md_docs_2runtime__analyzer.html#autotoc_md41", null ],
        [ "profile_runtime Function", "md_docs_2runtime__analyzer.html#autotoc_md42", null ]
      ] ],
      [ "Utility Functions", "md_docs_2runtime__analyzer.html#autotoc_md44", null ],
      [ "Technical Considerations", "md_docs_2runtime__analyzer.html#autotoc_md46", null ]
    ] ],
    [ "Runtime Reporter Module", "md_docs_2runtime__reporter.html", [
      [ "Key Features", "md_docs_2runtime__reporter.html#autotoc_md48", null ],
      [ "Core Components", "md_docs_2runtime__reporter.html#autotoc_md50", [
        [ "print_report Function", "md_docs_2runtime__reporter.html#autotoc_md51", null ],
        [ "save_report Function", "md_docs_2runtime__reporter.html#autotoc_md52", null ],
        [ "save_reports Function", "md_docs_2runtime__reporter.html#autotoc_md53", null ],
        [ "generate_report Function", "md_docs_2runtime__reporter.html#autotoc_md54", null ]
      ] ],
      [ "Technical Considerations", "md_docs_2runtime__reporter.html#autotoc_md56", [
        [ "Implementation Details", "md_docs_2runtime__reporter.html#autotoc_md57", null ],
        [ "Error Handling", "md_docs_2runtime__reporter.html#autotoc_md58", null ]
      ] ]
    ] ],
    [ "Sample Utilities Module", "md_docs_2sample__utilities.html", [
      [ "Key Features", "md_docs_2sample__utilities.html#autotoc_md60", null ],
      [ "Concepts and Type Requirements", "md_docs_2sample__utilities.html#autotoc_md62", null ],
      [ "Core Components", "md_docs_2sample__utilities.html#autotoc_md64", [
        [ "SampleSizeConfig Structure", "md_docs_2sample__utilities.html#autotoc_md65", null ],
        [ "generate_sizes Function", "md_docs_2sample__utilities.html#autotoc_md66", null ],
        [ "generate_samples Function", "md_docs_2sample__utilities.html#autotoc_md67", null ],
        [ "serialize_iterable Function", "md_docs_2sample__utilities.html#autotoc_md68", null ],
        [ "save_samples Function", "md_docs_2sample__utilities.html#autotoc_md69", null ]
      ] ],
      [ "Error Handling", "md_docs_2sample__utilities.html#autotoc_md71", null ],
      [ "Integration with Runtime Analyzer", "md_docs_2sample__utilities.html#autotoc_md73", null ]
    ] ],
    [ "Plot Generation Script", "md_docs_2plot__generation.html", [
      [ "Key Features", "md_docs_2plot__generation.html#autotoc_md75", null ],
      [ "File Validation and Error Handling", "md_docs_2plot__generation.html#autotoc_md77", null ],
      [ "Interactive Plotting and Report Capacity", "md_docs_2plot__generation.html#autotoc_md78", null ],
      [ "PNG File Generation and Naming Convention", "md_docs_2plot__generation.html#png_format", null ]
    ] ],
    [ "Output Formats", "md_docs_2output__formats.html", [
      [ "Format Detection", "md_docs_2output__formats.html#autotoc_md80", null ],
      [ "Compatibility", "md_docs_2output__formats.html#autotoc_md82", null ],
      [ "Runtime Report Formats", "md_docs_2output__formats.html#autotoc_md84", [
        [ "Text Format (.txt)", "md_docs_2output__formats.html#txt_format", null ],
        [ "CSV Format (.csv)", "md_docs_2output__formats.html#csv_format", null ],
        [ "JSON Format (.json)", "md_docs_2output__formats.html#json_format", null ]
      ] ],
      [ "Sample Data Formats", "md_docs_2output__formats.html#autotoc_md86", [
        [ "Text Format (.txt)", "md_docs_2output__formats.html#autotoc_md87", null ],
        [ "CSV Format (.csv)", "md_docs_2output__formats.html#autotoc_md88", null ],
        [ "JSON Format (.json)", "md_docs_2output__formats.html#autotoc_md89", null ]
      ] ]
    ] ],
    [ "Concepts", "concepts.html", "concepts" ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", null ],
        [ "Functions", "functions_func.html", null ],
        [ "Variables", "functions_vars.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"annotated.html"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';