import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('analysis.csv')

books = df['book'].unique()

performance_metrics = []

for book in books:
    book_data = df[df['book'] == book]
    word_count = book_data['word count'].iloc[0]
    
    metrics = {
        'book': book,
        'word_count': word_count,
        'avg_search_time_2char': book_data[book_data['size of prefix'] == 2]['time (ms)'].mean(),
        'avg_search_time_3char': book_data[book_data['size of prefix'] == 3]['time (ms)'].mean(),
        'avg_search_time_4char': book_data[book_data['size of prefix'] == 4]['time (ms)'].mean(),
        'avg_results_2char': book_data[book_data['size of prefix'] == 2]['number of results'].mean(),
        'avg_results_3char': book_data[book_data['size of prefix'] == 3]['number of results'].mean(),
        'avg_results_4char': book_data[book_data['size of prefix'] == 4]['number of results'].mean(),
    }
    performance_metrics.append(metrics)

metrics_df = pd.DataFrame(performance_metrics)
metrics_df = metrics_df.sort_values('word_count')

# search times vs word count
plt.figure(figsize=(12, 6))
for prefix in [2, 3, 4]:
    plt.plot(metrics_df['word_count'], 
             metrics_df[f'avg_search_time_{prefix}char'],
             marker='o',
             label=f'{prefix} char prefix')

# Add book labels
for idx, row in metrics_df.iterrows():
    plt.annotate(row['book'].replace('.txt', ''),
                (row['word_count'], row['avg_search_time_2char']),
                xytext=(5, 5), textcoords='offset points',
                fontsize=8)

plt.xlabel('Book Size (words)')
plt.ylabel('Average Search Time (ms)')
plt.title('Search Time vs Book Size')
plt.legend()
plt.tight_layout()
plt.savefig('./graphs/search_time_vs_size.png', dpi=300, bbox_inches='tight')
plt.close()

# number of results vs word count
plt.figure(figsize=(12, 6))
for prefix in [2, 3, 4]:
    plt.plot(metrics_df['word_count'],
             metrics_df[f'avg_results_{prefix}char'],
             marker='o',
             label=f'{prefix} char prefix')
    # Add book labels
    if prefix == 2:
        for idx, row in metrics_df.iterrows():
            plt.annotate(row['book'].replace('.txt', ''),
                        (row['word_count'], row[f'avg_results_{prefix}char']),
                        xytext=(5, 5), textcoords='offset points',
                        fontsize=8)
plt.xlabel('Book Size (words)')
plt.ylabel('Average Number of Results')
plt.title('Number of Results vs Book Size')
plt.legend()
plt.tight_layout()
plt.savefig('./graphs/results_vs_size.png', dpi=300, bbox_inches='tight')
plt.close()

# search times by prefix length
plt.figure(figsize=(12, 6))
metrics_df.plot(x='book', 
               y=['avg_search_time_2char', 'avg_search_time_3char', 'avg_search_time_4char'],
               kind='bar',
               width=0.8)
plt.xticks(range(len(metrics_df['book'])), 
          [b.replace('.txt', '') for b in metrics_df['book']], 
          rotation=45, 
          ha='right')
plt.ylabel('Average Search Time (ms)')
plt.title('Search Time by Prefix Length for Each Book')
plt.legend(['2 char', '3 char', '4 char'])
plt.tight_layout()
plt.savefig('./graphs/search_time_by_book.png', dpi=300, bbox_inches='tight')
plt.close()

# number of results by prefix length
plt.figure(figsize=(12, 6))
metrics_df.plot(x='book',
               y=['avg_results_2char', 'avg_results_3char', 'avg_results_4char'],
               kind='bar',
               width=0.8)
plt.xticks(range(len(metrics_df['book'])), 
          [b.replace('.txt', '') for b in metrics_df['book']], 
          rotation=45, 
          ha='right')
plt.ylabel('Average Number of Results')
plt.title('Number of Results by Prefix Length for Each Book')
plt.legend(['2 char', '3 char', '4 char'])
plt.tight_layout()
plt.savefig('./graphs/results_by_book.png', dpi=300, bbox_inches='tight')
plt.close()