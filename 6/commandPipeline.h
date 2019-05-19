#ifndef COMMANDPIPELINE_H
#define COMMANDPIPELINE_H

// Запуск конвейера команд: 
// tr -d "[0-9]" < input_filename | sort | uniq > output_filename
void startComminication(const char* input_filename, const char* output_filename);

#endif
