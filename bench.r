library(ggplot2)
library(tidyr)
library(dplyr)
library(readr)

setwd("~/Research/COMP-522/")



################## X86 - i9-12900K ################## 

# Analysis of the benchmarking results
df_x86 <-  read.table("bench_x86.tsv", header=TRUE, sep='\t') %>%
  group_by(memory_model, n_threads) %>%
  summarise(avg_dur_s = mean(dur_ms) / 1000)


# Plot the comparison
ggplot(df_x86, aes(x=n_threads, y=avg_dur_s, color=memory_model)) +
  geom_line(linewidth=1.5) + 
  geom_point() + 
  theme(axis.title =element_text(size = rel(3.2)),
        axis.text = element_text(colour = "black", size = rel(2.4)),
        strip.text = element_text(size = rel(2.4))) +
  xlab("Number of threads") + ylab("Duration (s)") + labs(group="Memory Model") 
  

################## ARM - M1 Pro ################## 

df_mac <-  read.table("bench_mac.tsv", header=TRUE, sep='\t') %>%
  group_by(memory_model, n_threads) %>%
  summarise(avg_dur_s = mean(dur_ms) / 1000)

# Plot the comparison
ggplot(df_mac, aes(x=n_threads, y=avg_dur_s, color=memory_model)) +
  geom_line(linewidth=1.5) + 
  geom_point() + 
  theme(axis.title =element_text(size = rel(3.2)),
        axis.text = element_text(colour = "black", size = rel(2.4)),
        strip.text = element_text(size = rel(2.4))) +
  xlab("Number of threads") + ylab("Duration (s)") + labs(group="Memory Model") 

################## IBM - POWER9 ################## 

### Case with n_iter = 1000 and up to 20 threads
df_power9 <-  read.table("bench_power9.tsv", header=TRUE, sep='\t') %>%
  group_by(memory_model, n_threads) %>%
  summarise(avg_dur_s = mean(dur_ms-tot_dur_work_ms) / 1000)

# Plot the comparison
ggplot(df_power9, aes(x=n_threads, y=avg_dur_s, color=memory_model)) +
  geom_line(linewidth=1.5) + 
  geom_point() + 
  theme(axis.title =element_text(size = rel(3.2)),
        axis.text = element_text(colour = "black", size = rel(2.4)),
        strip.text = element_text(size = rel(2.4))) +
  xlab("Number of threads") + ylab("Duration (s)") + labs(group="Memory Model") 

### Case with n_iter = 1000 and up to 151 threads
df_power9_1000 <-  read.table("bench_power9_1000.tsv", header=TRUE, sep='\t') %>%
  group_by(memory_model, n_threads) %>%
  summarise(avg_dur_s = mean(dur_ms-tot_dur_work_ms) / 1000)

# Plot the comparison
ggplot(df_power9_1000, aes(x=n_threads, y=avg_dur_s, color=memory_model)) +
  geom_line(linewidth=1.5) + 
  geom_point() + 
  theme(axis.title =element_text(size = rel(3.2)),
        axis.text = element_text(colour = "black", size = rel(2.4)),
        strip.text = element_text(size = rel(2.4))) +
  xlab("Number of threads") + ylab("Duration (s)") + labs(group="Memory Model") 

### Case with n_iter = 20 and up to 160 threads
df_power9_20_50M <-  read.table("bench_power9_20_50M.tsv", header=TRUE, sep='\t') %>%
  group_by(memory_model, n_threads) %>%
  summarise(avg_dur_s = mean(dur_ms-tot_dur_work_ms) / 1000)

# Plot the comparison
ggplot(df_power9_20_50M, aes(x=n_threads, y=avg_dur_s, color=memory_model)) +
  geom_line(linewidth=1.5) + 
  geom_point() + 
  theme(axis.title =element_text(size = rel(3.2)),
        axis.text = element_text(colour = "black", size = rel(2.4)),
        strip.text = element_text(size = rel(2.4))) +
  xlab("Number of threads") + ylab("Duration (s)") + labs(group="Memory Model") 


### Case with n_iter = 100, 1M ops and up to 160 threads
df_power9_100_1M <-  read.table("bench_power9_100_1M.tsv", header=TRUE, sep='\t') %>%
  group_by(memory_model, n_threads) %>%
  summarise(avg_dur_s = mean(dur_ms-tot_dur_work_ms) / 1000)

# Plot the comparison
ggplot(df_power9_100_1M, aes(x=n_threads, y=avg_dur_s, color=memory_model)) +
  geom_line(linewidth=1.5) + 
  geom_point() + 
  theme(axis.title =element_text(size = rel(3.2)),
        axis.text = element_text(colour = "black", size = rel(2.4)),
        strip.text = element_text(size = rel(2.4))) +
  xlab("Number of threads") + ylab("Duration (s)") + labs(group="Memory Model") 



########### NEW RESULTS ########### 

# x86
df_x86_1000_10M <-  read.table("bench_x86_1000_10M.tsv", header=TRUE, sep='\t') %>%
  group_by(memory_model, n_threads) %>%
  summarise(avg_dur_s = mean(dur_ms-tot_dur_work_ms) / 1000)

# Plot the comparison
ggplot(df_x86_1000_10M, aes(x=n_threads, y=avg_dur_s, color=memory_model)) +
  geom_line(linewidth=1.5) + 
  geom_point() + 
  theme(axis.title =element_text(size = rel(2)),
        axis.text = element_text(colour = "black", size = rel(1)),
        strip.text = element_text(size = rel(1)), 
        legend.text=element_text(size=rel(1)),
        legend.title = element_text(size = rel(2))) +
  xlab("Number of threads") + ylab("Duration (s)") + labs(color="Memory Model") 
ggsave("res-x86.pdf", height = 6, width = 15)

df_x86_1000_10M_weak <- df_x86_1000_10M %>%
  filter(memory_model == "weak") %>%
  rename(dur_weak_ms = avg_dur_s)

df_x86_1000_10M_strong <- df_x86_1000_10M %>%
  filter(memory_model == "strong") %>%
  rename(dur_strong_ms = avg_dur_s)

df_x86_all <- merge(df_x86_1000_10M_weak, df_x86_1000_10M_strong, by="n_threads") %>%
  select(-c(memory_model.x, memory_model.y)) %>%
  mutate(percent_speedup = (1-dur_weak_ms/dur_strong_ms) * 100)

avg_speedup_x86 <- mean(df_x86_all$percent_speedup)


# POWER9
df_power9_1000_10M <-  read.table("bench_power9_1000_10M.tsv", header=TRUE, sep='\t') %>%
  group_by(memory_model, n_threads) %>%
  summarise(avg_dur_s = mean(dur_ms-tot_dur_work_ms) / 1000)

# Plot the comparison
ggplot(df_power9_1000_10M, aes(x=n_threads, y=avg_dur_s, color=memory_model)) +
  geom_line(linewidth=1.5) + 
  geom_point() + 
  theme(axis.title =element_text(size = rel(3.2)),
        axis.text = element_text(colour = "black", size = rel(2.4)),
        strip.text = element_text(size = rel(2.4))) +
  xlab("Number of threads") + ylab("Duration (s)") + labs(color="Memory Model") 
ggsave("res-power9.pdf", height = 6, width = 15)

df_power9_weak <- df_power9_1000_10M %>%
  filter(memory_model == "weak") %>%
  rename(dur_weak_ms = avg_dur_s)

df_power9_strong <- df_power9_1000_10M %>%
  filter(memory_model == "strong") %>%
  rename(dur_strong_ms = avg_dur_s)

df_power9_all <- merge(df_power9_weak, df_power9_strong, by="n_threads") %>%
  select(-c(memory_model.x, memory_model.y)) %>%
  mutate(percent_speedup = (1-dur_weak_ms/dur_strong_ms) * 100)

avg_speedup_power9 <- mean(df_power9_all$percent_speedup)

# Multithreading results
df_power9_1000_10M_320 <-  read.table("bench_power9_1000_10M_320.tsv", header=TRUE, sep='\t') %>%
  group_by(memory_model, n_threads) %>%
  summarise(avg_dur_s = mean(dur_ms-tot_dur_work_ms) / 1000)

# Plot the comparison
ggplot(df_power9_1000_10M_320, aes(x=n_threads, y=avg_dur_s, color=memory_model)) +
  geom_line(linewidth=1.5) + 
  geom_point() + 
  theme(axis.title =element_text(size = rel(3.2)),
        axis.text = element_text(colour = "black", size = rel(2.4)),
        strip.text = element_text(size = rel(2.4))) +
  xlab("Number of threads") + ylab("Duration (s)") + labs(color="Memory Model") 
ggsave("res-power9-multithreading.pdf", height = 6, width = 15)

df_power9_320_weak <- df_power9_1000_10M_320 %>%
  filter(memory_model == "weak") %>%
  rename(dur_weak_ms = avg_dur_s)

df_power9_320_strong <- df_power9_1000_10M_320 %>%
  filter(memory_model == "strong") %>%
  rename(dur_strong_ms = avg_dur_s)

df_power9_320_all <- merge(df_power9_320_weak, df_power9_320_strong, by="n_threads") %>%
  select(-c(memory_model.x, memory_model.y)) %>%
  mutate(percent_speedup = (1-dur_weak_ms/dur_strong_ms) * 100) %>% 
  filter(n_threads >= 160)

avg_speedup_power9_320 <- mean(df_power9_320_all$percent_speedup)

# Mac M1 Pro
df_mac <-  read.table("bench_mac_1000_10M.tsv", header=TRUE, sep='\t') %>%
  group_by(memory_model, n_threads) %>%
  summarise(avg_dur_s = mean(dur_ms-tot_dur_work_ms) / 1000)

# Plot the comparison
ggplot(df_mac, aes(x=n_threads, y=avg_dur_s, color=memory_model)) +
  geom_line(linewidth=1.5) + 
  geom_point() + 
  theme(axis.title =element_text(size = rel(3.2)),
        axis.text = element_text(colour = "black", size = rel(2.4)),
        strip.text = element_text(size = rel(2.4))) +
  xlab("Number of threads") + ylab("Duration (s)") + labs(color="Memory Model") 
ggsave("res-m1pro-multithreading.pdf", height = 6, width = 15)

df_mac_weak <- df_mac %>%
  filter(memory_model == "weak") %>%
  rename(dur_weak_ms = avg_dur_s)

df_mac_strong <- df_mac %>%
  filter(memory_model == "strong") %>%
  rename(dur_strong_ms = avg_dur_s)

df_mac_all <- merge(df_mac_weak, df_mac_strong, by="n_threads") %>%
  select(-c(memory_model.x, memory_model.y)) %>%
  mutate(percent_speedup = (1-dur_weak_ms/dur_strong_ms) * 100)

avg_speedup_mac<- mean(df_mac_all$percent_speedup)

