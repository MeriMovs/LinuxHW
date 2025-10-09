import random
import string

def generate_random_word(min_length=1, max_length=10):
    """Generate a random word of variable length."""
    length = random.randint(min_length, max_length)
    return ''.join(random.choices(string.ascii_letters, k=length))

def generate_text_file(filename="text", target_size_mb=1):
    """Generate a text file of approximately target_size_mb MB."""
    target_size_bytes = target_size_mb * 1024 * 1024  # Convert MB to bytes
    current_size = 0
    lines = []
    
    while current_size < target_size_bytes:
        # Generate a line with 1 to 10 random words
        num_words = random.randint(1, 10)
        line = ' '.join(generate_random_word() for _ in range(num_words)) + '\n'
        lines.append(line)
        current_size += len(line.encode('utf-8'))  # Estimate size in bytes
    
    # Write to file
    with open(filename, 'w') as f:
        for line in lines:
            f.write(line)
            # Stop if we exceed target size to avoid overshooting too much
            if f.tell() >= target_size_bytes:
                break
    
    print(f"Generated file '{filename}' with size ~{f.tell() / (1024 * 1024):.2f} MB")

# Generate the 50 MB text file
generate_text_file("text", 1)